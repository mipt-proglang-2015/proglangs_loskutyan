__author__ = 'loskutyan'

from  xml.dom.minidom import parse
import numpy as np
import sys
import time
import FloydWarshall

def read_data(input):
    data = parse(input)
    return {'net' : data.getElementsByTagName('net'),
            'resistors' : data.getElementsByTagName('resistor'),
            'capactors' : data.getElementsByTagName('capactor'),
            'diodes' : data.getElementsByTagName('diode')}

def get_edges_from_record(record, is_diode):
    attrs = record.attributes
    source = int(attrs['net_from'].value) - 1
    target = int(attrs['net_to'].value) - 1
    resistance = float(attrs['resistance'].value)
    yield (source, target, resistance)

    if is_diode:
        reverse_resistance = float(attrs['reverse_resistance'].value)
        yield (target, source, reverse_resistance)

def add_edge_to_matrix(matrix, edge):
    source, target, resistance = edge[0], edge[1], edge[2]
    if matrix[source][target]:
        matrix[source][target] = (matrix[source][target] * resistance) / (matrix[source][target] + resistance)
    else:
        matrix[source][target] = resistance


def build_matrix(data):
    net_len = len(data['net'])
    matrix = np.zeros((net_len, net_len))

    for record in data['resistors']:
        for edge in get_edges_from_record(record, False):
            add_edge_to_matrix(matrix, edge)

    for record in data['capactors']:
        for edge in get_edges_from_record(record, False):
            add_edge_to_matrix(matrix, edge)

    for record in data['diodes']:
        for edge in get_edges_from_record(record, True):
            add_edge_to_matrix(matrix, edge)

    return matrix.tolist()

def write_matrix(output, matrix):
    with open(output, 'w') as f:
        f.write('\n'.join(map(lambda row: '\t'.join(map(lambda x: str(round(x, 4)), row)), matrix)))

def main():
    if len(sys.argv) != 3:
        print ('Incorrect input format\n')
    else:
        start = time.process_time()
        write_matrix(sys.argv[2], FloydWarshall.calculate(build_matrix(read_data(sys.argv[1]))))
        end = time.process_time()
        print("Time consumption: {} sec".format((end - start)))

