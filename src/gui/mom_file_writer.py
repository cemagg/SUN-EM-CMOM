import datetime
from data_structures import Node
from data_structures import Triangle
from data_structures import Edge

def writeMoMFIle(file_name, const, nodes, triangles, edges):
    file = open(file_name, 'w+')

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

    file.close()