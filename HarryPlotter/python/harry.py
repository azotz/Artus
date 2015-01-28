#!/usr/bin/env python
# -*- coding: utf-8 -*-

import logging
import Artus.Utility.logger as logger
log = logging.getLogger(__name__)

import collections
from multiprocessing import Pool

import Artus.Utility.jsonTools as jsonTools
import Artus.HarryPlotter.core as harrycore


def pool_plot(args):
	try:
		args[0].plot(*args[1:])
		return None
	except SystemExit as e:
		return args[1:]
	except Exception as e:
		log.info(str(e))
		return args[1:]

class HarryPlotter(object):
	def __init__(self, list_of_config_dicts=None, list_of_args_strings=None, n_processes=1):
		self.multi_plots(list_of_config_dicts=list_of_config_dicts,
		                 list_of_args_strings=list_of_args_strings,
		                 n_processes=n_processes)
	
	def plot(self, harry_args):
		harry_core = harrycore.HarryCore(args_from_script=harry_args)
		harry_core.run()
	
	def multi_plots(self, list_of_config_dicts, list_of_args_strings, n_processes=1):
		config_dicts = list_of_config_dicts if isinstance(list_of_config_dicts, collections.Iterable) and not isinstance(list_of_config_dicts, basestring) else [list_of_config_dicts]
		args_strings = list_of_args_strings if isinstance(list_of_args_strings, collections.Iterable) and not isinstance(list_of_args_strings, basestring) else [list_of_args_strings]
		
		# fill up missing list elements by repeating previous ones
		n_plots = max(len(config_dicts), len(args_strings))
		if (len(config_dicts) > 1) and (len(config_dicts) < n_plots):
			log.warning("Too few config dicts specified!")
		if (len(args_strings) > 1) and (len(args_strings) < n_plots):
			log.warning("Too few argument lists specified!")
		config_dicts = (config_dicts*n_plots)[:n_plots]
		args_strings = (args_strings*n_plots)[:n_plots]
		
		harry_args = []
		for config_dict, args_string in zip(config_dicts, args_strings):
			if config_dict is None:
				harry_args.append(None)
			else:
				harry_args.append("--json-defaults \"%s\"" % jsonTools.JsonDict(config_dict).toString(indent=None).replace("\"", "'"))
			
			if not args_string is None:
				if harry_args[-1] is None:
					harry_args[-1] = args_string
				else:
					harry_args[-1] += (" "+args_string)
		
		# multi processing of multiple plots
		failed_plots = []
		if len(harry_args) > 1 and n_processes > 1:
			pool = Pool(processes=n_processes)
			results = pool.map_async(pool_plot, zip([self]*len(harry_args), harry_args))
			failed_plots = [result for result in results.get() if not result is None and result != (None,)]
		
		# single processing of multiple plots
		elif len(harry_args) > 1:
			for harry_args in harry_args:
				try:
					self.plot(harry_args)
				except SystemExit as e:
					failed_plots.append(harry_args)
				except Exception as e:
					log.info(str(e))
					failed_plots.append(harry_args)
		
		# single plot
		elif len(harry_args) > 0:
			self.plot(harry_args[0])
		
		if len(failed_plots) > 0:
			log.error("%d failed plots:" % len(failed_plots))
			for failed_plot in failed_plots:
				log.info("\t%s" % failed_plot)
