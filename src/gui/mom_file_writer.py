import datetime
from data_structures import Node
from data_structures import Triangle
from data_structures import Edge

def writeMoMFIle(file_name, const, nodes, triangles, edges, excitations):
    file = open(file_name, 'w+', newline="\n")

    # write header
    file.write("CMoM INPUT FILE\n")
    file.write(str(datetime.datetime.now()) + "\n\n")

    # write const
    file.write("CONST START\n")
    file.write("%-30s\t\t%d\n\n" % ("NUM_FIELDS", len(const)))

    for entry in const:
        file.write("%-30s\t\t%s\n" % (entry, str(const[entry])))

    file.write("CONST END\n\n")

    file.write("FEKO_DATA START\n")

    # write nodes
    file.write("NODES START\n")
    file.write("%-20s\t\t%d\n" % ("NUM_NODES", len(nodes)))
    file.write("%s\t\t%s\t\t%s\n" % ("X_COORD", "Y_COORD", "Z_COORD"))

    for node in nodes:
        file.write("%f\t%f\t%f\n" % (node.x, node.y, node.z))
    
    file.write("NODES END\n\n")

    # write triangles
    file.write("TRIANGLES START\n")
    file.write("%-20s\t\t%d\n\n" % ("NUM_TRIANGLES", len(triangles)))
    file.write("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n" % ("V", "V", "V", "CENTRE_X", "CENTRE_Y", "CENTRE_Z", "AREA", "LABEL"))

    for tri in triangles:
        file.write("%d\t%d\t%d\t%f\t%f\t%f\t%f\t%d\n" % (tri.v1,
                                                         tri.v2,
                                                         tri.v3,
                                                         tri.centre.x,
                                                         tri.centre.y,
                                                         tri.centre.z,
                                                         tri.area,
                                                         tri.label))
    
    file.write("TRIANGLES END\n\n")
  
    # write edges
    file.write("EDGES START\n")
    file.write("%-20s\t\t%d\n\n" % ("NUM_EDGES", len(edges)))

    
    file.write("%s\t%s\t%8s\t%8s\t%8s\t%8s\t%s\t%s\t%s\t%s\t%8s\t%8s\t%8s\t%8s\t%8s\t%8s\n" % ("V",
                                                                                                        "V",
                                                                                                        "CENTRE_X",
                                                                                                        "CENTRE_Y",
                                                                                                      "CENTRE_Z",
                                                                                                      "LENGTH",
                                                                                                      "TM",
                                                                                                      "TP",
                                                                                                      "MF",
                                                                                                      "PF",
                                                                                                      "RHOCMX",
                                                                                                      "RHOCMY",
                                                                                                      "RHOCMZ",
                                                                                                      "RHOCPX",
                                                                                                      "RHOCPY",
                                                                                                      "RHOCPZ")) 
    
    for edge in edges:
        file.write("%d\t%d\t%8f\t%8f\t%8f\t%8f\t%d\t%d\t%d\t%d\t%8f\t%8f\t%8f\t%8f\t%8f\t%8f\n" % (edge.v1,
                                                                                                   edge.v2,
                                                                                                   edge.centre.x,
                                                                                                   edge.centre.y,
                                                                                                   edge.centre.z,
                                                                                                   edge.length,
                                                                                                   edge.tm,
                                                                                                   edge.tp,
                                                                                                   edge.mf,
                                                                                                   edge.pf,
                                                                                                   edge.rcm.x,
                                                                                                   edge.rcm.y,
                                                                                                   edge.rcm.z,
                                                                                                   edge.rcp.x,
                                                                                                   edge.rcp.y,
                                                                                                   edge.rcp.z))
    
    file.write("EDGES END\n")
    file.write("FEKO_DATA END\n\n")

    # write excitations
    file.write("EXCITATIONS START\n")
    file.write("%-20s\t\t%d\n\n" % ("NUM_EXCITATIONS", len(excitations)))
    
    
    for excitation in excitations:
        label = -1

        if excitation.type == 2:
            for port in excitation.ports:
                tmp_label = triangles[edges[abs(port)].tm].label

                if label == -1:
                    label = tmp_label
                else:
                    if tmp_label != label:
                        quit()


        file.write("%-30s\t\t%s\n" % ("index", str(excitation.index)))
        file.write("%-30s\t\t%s\n" % ("type", str(excitation.type)))
        file.write("%-30s\t\t%s\n" % ("label", str(label)))
        file.write("%-30s\t\t%s\n" % ("emag", str(excitation.emag)))
        
        if excitation.type == 1:
            file.write("%-30s\t\t%s\n" % ("theta", str(excitation.theta)))
            file.write("%-30s\t\t%s\n" % ("phi", str(excitation.phi)))
        
        elif excitation.type == 2:
            file.write("%-30s\t\t%s\n" % ("numPorts", str(len(excitation.ports))))
            
            for port in excitation.ports:
                file.write("%-30s\t\t%s\n" % ("port", str(port)))
        
        file.write("\n")
    
    file.write("EXCITATIONS END\n")

    file.close()