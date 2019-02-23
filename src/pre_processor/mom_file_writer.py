import datetime

def writeMoMFIle(file_name, const, nodes, triangles, edges):
    file = open(file_name, 'w+')

    # write header
    file.write("CMoM INPUT FILE\n")
    file.write(str(datetime.datetime.now()) + "\n\n")

    # write const
    file.write("CONST START\n")
    file.write("%-30s\t\t%d\n\n" % ("NUM_FIELDS", len(const)))

    # write nodes
     
    # write triangles
    
    # write edges 

    file.close()