import argparse, sys, time, csv, time
import numpy as np
import pyblockset
from pyblockset import Config

def parseCmdArgs():
    parser = argparse.ArgumentParser()

    parser.add_argument('--mode', action='store', dest='mode',
                    help='<inference/pack/both>')

    parser.add_argument('--layout', action='store', dest='layout', 
                    help='<bfs/dfs/statdfs/statblockdfs/binbfs/binbfs/binstatdfs>')

    parser.add_argument('--intertwine',  dest='intertwine', nargs='?', default='3',
                    help='', const='3')
    
    parser.add_argument('--numbins',  dest='num_bins', nargs='?', default='1',
                    help='', const='1')
    
    parser.add_argument('--numthreads', dest='num_threads', nargs='?', default='1',
                    help='', const='1')

    parser.add_argument('--numtrees', action='store', dest='num_trees',
                    help='Number of trees')

    parser.add_argument('--packfilename', action='store', dest='pack_filename', nargs='?',
                    help='Filepath of the saved model', const='None')

    parser.add_argument('--datafilename', action='store', dest='data_filename', nargs='?',
                    const='None', help='Data filename')
    
    parser.add_argument('--numfiles', dest='num_files', nargs='?', default='1',
                    const='1', help='Data filename')

    parser.add_argument('--modelfilename', action='store', dest='model_filename', nargs='?',
                    const='None.pkl', help='Filepath of the model to be read')
    
    parser.add_argument('--metadatafilename', action='store', dest='metadata_filename', nargs='?',
                    const='None.pkl', help='Filepath of the model to be read')
    
    parser.add_argument('--algorithm', action='store', dest='algorithm', nargs='?',
                    const='randomforest', help='randomforest/gradientboost')
    
    parser.add_argument('--task', action='store', dest='task', nargs='?',
                    const='classification/regression', help='classification/regression')
    
    parser.add_argument('--batchsize', dest='batch_size', nargs='?', default='1',
                    const='1', help='batch size')
    
    parser.add_argument('--labelcol', dest='label_col', nargs='?', default='1',
                    const='1', help='')
    
    parser.add_argument('--format',  dest='format', nargs='?', default='1',
                    const='binary', help='binary/text')

    parser.add_argument('--blocksize',  dest='blocksize', nargs='?', default='128',
                    const='128', help='Block size')

    results = parser.parse_args()

    return results


args = parseCmdArgs()
Config.setConfigItem('mode', args.mode)
Config.setConfigItem('layout', args.layout)
Config.setConfigItem('intertwine', args.intertwine)
Config.setConfigItem('numbins', args.num_bins)
Config.setConfigItem('numtrees', args.num_trees)
Config.setConfigItem('numthreads', args.num_threads)
Config.setConfigItem('packfilename', args.pack_filename)
Config.setConfigItem('datafilename', args.data_filename)
Config.setConfigItem('numfiles', args.num_files)
Config.setConfigItem('batchsize', args.batch_size)
Config.setConfigItem('labelcol', args.label_col)
Config.setConfigItem('modelfilename', args.model_filename)
Config.setConfigItem('metadatafilename', args.metadata_filename)
Config.setConfigItem('algorithm', args.algorithm)
Config.setConfigItem('task', args.task)
Config.setConfigItem('format', args.format)
Config.setConfigItem('blocksize', args.blocksize)

model = pyblockset.PacsetBase()

if Config.getValue('algorithm') == 'randomforest':
    if Config.getValue('task') == 'classification':
        print("rf")
        model.initRandomForestClassifier()
    else:
        model.initRandomForestRegressor()
else:
    if Config.gettValue('task') == 'classification':
        model.initGradientBoostedClassifier()
    else:
        model.initGradientBoostedRegressor()

if Config.getValue('mode') == 'pack':
    model.loadAndPack()
else:
    model.predict()

