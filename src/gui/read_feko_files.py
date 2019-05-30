from data_structures import *
from log import log
import os
import subprocess
import numpy as np

import math

class FEKOFileReader:

    def __init__(self, file_name):
        self.file_name = file_name
        self.nodes = []
        self.triangles = []
        self.edges = []
        self.const = {}
        self.excitations = []
    
    def readFEKOOutFile(self):
        
        self.const["srcFile"] = self.file_name
        self.const["numFreq"] = 0
        self.const["freq"] = []
        self.const["numPortEdges"] = 0

        try:
            file = open(self.file_name, "r")
            log(self.file_name + " opened successfully")
        except IOError as e:
            log(e)
            return False

        line = file.readline()

        # Loop through file for the first time to get number of triangles, edges and frequencies 
        while line:

            # Get number of triangles
            if "Number of metallic triangles:" in line:
                content = line.split()
                number_of_triangles = int(content[9])
                
            # Get number of edges
            if "Number of metallic edges (MoM):" in line:
                content = line.split()
                number_of_edges = int(content[13])

            if "EXCITATION BY VOLTAGE SOURCE AT AN EDGE" in line:
                portEdges = []

                line = file.readline() # read empty line
                line = file.readline() # read name
                line = file.readline() # read index
                content = line.split()
                excitation = Excitation(int(content[2]) - 1)
                
                line = file.readline() # read freq
                content = line.split()

                if not self.excitations:
                    self.const["numFreq"] = self.const["numFreq"] + 1
                    self.const["freq"].append(float(content[5]))
                else:
                    if float(content[5]) not in self.const["freq"]:
                        self.const["numFreq"] = self.const["numFreq"] + 1
                        self.const["freq"].append(float(content[5]))

                line = file.readline() # read wavelength
                
                line = file.readline() # read voltage
                content = line.split()
                emag = float(content[7])
                
                line = file.readline() # read phase
                line = file.readline() # read attached port
                line = file.readline() # read port edge length
                if "between" in line:
                    line = file.readline()
                    line = file.readline()
                    line = file.readline()

                line = file.readline() # read edge indices
                content = line.split()

                for i in range(4,len(content)):
                    if int(content[i]) < 0:
                        port = int(content[i]) + 1
                    else:
                        port = int(content[i]) - 1

                    portEdges.append(port)
                
                excitation.edgePort(portEdges, emag)
                self.excitations.append(excitation)

            if "EXCITATION BY INCIDENT PLANE ELECTROMAGNETIC WAVE" in line:
                line = file.readline() # read empty line
                line = file.readline() # read name
                line = file.readline() # read index
                content = line.split()
                excitation = Excitation(int(content[2]) - 1)


                line = file.readline() # read freq
                content = line.split()

                if not self.excitations:
                    self.const["numFreq"] = self.const["numFreq"] + 1
                    self.const["freq"].append(float(content[5]))
                else:
                    if float(content[5]) not in self.const["freq"]:
                        self.const["numFreq"] = self.const["numFreq"] + 1
                        self.const["freq"].append(float(content[5]))

                line = file.readline() # read wavelength

                line = file.readline() # read direction of incidence
                content = line.split()
                phi = content[8]
                theta = content[5]
                
                line = file.readline() # read polarisation
                line = file.readline() # read axial ratio
                line = file.readline() # read polarisation angle
                line = file.readline() # read direction of propagation 1
                line = file.readline() # read direction of propagation 2
                line = file.readline() # read direction of propagation 3
                line = file.readline() # read wave number

                line = file.readline() # read field strength 1
                content = line.split()
                e_x = float(content[6])

                line = file.readline() # read field strength 2
                content = line.split()
                e_y = float(content[5])

                line = file.readline() # read field strength 3
                content = line.split()
                e_z = float(content[2])

                emag = math.sqrt(pow(e_x, 2) + pow(e_y, 2) + pow(e_z, 2))

                excitation.planeWave(theta, phi, emag)
                self.excitations.append(excitation)

            # Get next line
            line = file.readline() 

        # Close the file
        file.close()
        self.const["numEdges"] = number_of_edges
        print("After first pass through") 

        # Second pass through file to read edge, triangle and source data
        file = open(self.file_name, "r")
        line = file.readline()

        while line:
            
            # Get the triangle data
            if "DATA OF THE METALLIC TRIANGLES" in line:
                print("STARTING TRIANGLES---------------------------------------------------------")
                line = file.readline() # empty line
                line = file.readline() # first header
                line = file.readline() # second header
                line = file.readline() # third header
                line = file.readline() # fourth header

                label_list = []

                for i in range(number_of_triangles):
                    print(i)
                    line = file.readline() # row no label x1 y1 z1 edges 
                    content = line.split()

                    if "@" in content[1]:
                        content[1] = content[1][content[1].index("@"):]

                    if content[1] in label_list:
                        label = label_list.index(content[1])
                    else:
                        label_list.append(content[1])
                        label = label_list.index(content[1])

                    current_nodes = []
                    current_nodes.append(Node(float(content[2]), float(content[3]), float(content[4])))

                    line = file.readline() # row x2 y2 z2
                    content = line.split()
                    current_nodes.append(Node(float(content[2]), float(content[3]), float(content[4])))

                    line = file.readline() # row x3 y3 z3
                    content = line.split()
                    current_nodes.append(Node(float(content[2]), float(content[3]), float(content[4])))

                    line = file.readline() # row nx ny nz area
                    content = line.split()
                    area = float(content[3])

                    current_vertices = []
                    for i in range(3):
                        if current_nodes[i] in self.nodes:
                            current_vertices.append(self.nodes.index(current_nodes[i]))
                        else:
                            self.nodes.append(current_nodes[i])
                            current_vertices.append(self.nodes.index(current_nodes[i]))
                    
                    self.triangles.append(Triangle(current_vertices[0],
                                                   current_vertices[1],
                                                   current_vertices[2],
                                                   triangleCentreCalculator(current_nodes),
                                                   area,
                                                   label))
                    
            if "DATA OF THE METALLIC EDGES" in line:
                print("STARTING EDGES---------------------------------------------------------")
                line = file.readline() # empty line
                line = file.readline() # first header
                line = file.readline() # second header

                for i in range(number_of_edges):
                    print(i)
                    line = file.readline() # no type length KORP KORM
                    content = line.split()

                    length = float(content[2])
                    korp = int(content[6]) - 1 # plus triangle
                    korm = int(content[7]) - 1 # minus triangle

                    edge_vertices = getEdgeVertices(self.triangles[korp], self.triangles[korm])
                    

                    centre = edgeCentreCalculator(self.nodes[edge_vertices[0]], self.nodes[edge_vertices[1]])

                    plus_free_vertex = egdeGetFreeVertex(self.triangles[korp], edge_vertices)
                    minus_free_vertex = egdeGetFreeVertex(self.triangles[korm], edge_vertices)

                    rho_c_plus = edgeRhoPlusMinusCalculator(self.triangles[korp], self.nodes[plus_free_vertex], 0)
                    rho_c_minus = edgeRhoPlusMinusCalculator(self.triangles[korm], self.nodes[minus_free_vertex], 1)

                    self.edges.append(Edge(edge_vertices[0],
                                           edge_vertices[1],
                                           centre,
                                           length,
                                           korm,
                                           korp,
                                           minus_free_vertex,
                                           plus_free_vertex,
                                           rho_c_minus,
                                           rho_c_plus))

            
            line = file.readline()
        
        # check if all geometric elements are the same
        print("STARTING LABELS---------------------------------------------------------")
        same_element_label_index = []
        for item in label_list:
            if "@" in item:
                same_element_label_index.append(label_list.index(item))
        
        self.const["sameElem"] = same_element_label_index
        print("after second pass through")
        return True

def readFEKOStrFile(file_name, cwd):

    cwd = cwd + "bin/"
    file_separator = file_name.rsplit('/', 1) 
    file_separator[1] = "ascii_" + file_separator[1]

    ascii_file_name = file_separator[0] + '/' + file_separator[1]

    if os.path.isfile(ascii_file_name) != True:
        cmd = "%sstr2ascii %s -r > %s 2>&1" % (cwd, file_name, ascii_file_name)
        process = subprocess.Popen(cmd,stdout=subprocess.PIPE, shell=True)
        proc_stdout = process.communicate()[0].strip()

    file = open(ascii_file_name, "r")
    line = file.readline()
    line = file.readline()
    line = file.readline()
    num_isol = int(line)
    line = file.readline()
    line = file.readline()

    isol = np.empty(num_isol, dtype=complex)
    for i in range(num_isol):
        line = file.readline()
        content = line.split()
        isol[i] = np.complex(float(content[1][:-1]), float(content[2][:-1]))
    
    return isol

def errNormPercentage(isol, iref):
    diff_sum = 0
    ref_sum = 0

    for i in range(isol.size):
        diff_sum = diff_sum + np.power((np.abs(isol[i] - iref[i])), 2)
        ref_sum = ref_sum + np.power(np.abs(iref[i]), 2)

    return (np.sqrt(diff_sum) / np.sqrt(ref_sum)) * 100



if __name__ == "__main__":
    # freader = FEKOFileReader("C:\\Users\\Tameez\\Documents\\git\\SUN-EM\\examples\\two_plate_array\\pec_plate.out")
    freader = FEKOFileReader("C:\\Users\\Tameez\\Documents\\git\\SUN-EM-CMOM\\test\\test_cases\\strip_dipole_array\\strip_dipole_array.out")
    freader.readFEKOOutFile()


#readFEKOStrFile("/home/tameez/github/SUN-EM-CMOM/examples/two_plate_array/two_plate_array_coarse.str")
