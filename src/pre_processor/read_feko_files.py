class FEKOFileReader:

    def __init__(self, file_name):
        self.file_name = file_name

    
    def readFEKOOutFile(self):
        file = open(self.file_name, "r")
        line = file.readline()

        # Loop through file for the first time to get number of triangles, edges and frequencies 
        while line:

            # Get number of triangles
            if "Number of metallic triangles:" in line:
                contents = line.split()
                number_of_triangles = int(contents[9])
                
            # Get number of edges
            if "Number of metallic edges (MoM):" in line:
                contents = line.split()
                number_of_edges = int(contents[13])

 
            # get frequency

            # Get next line
            line = file.readline() 

        # Close the file
        file.close()

        # Second pass through file to read edge, triangle and source data
        file = open(self.file_name, "r")
        line = file.readline()

        while line:
            
            # Get the triangle data
            if "DATA OF THE METALLIC TRIANGLES" in line:
                line = file.readline() # empty line
                line = file.readline() # first header
                line = file.readline() # second header
                line = file.readline() # third header
                line = file.readline() # fourth header
                
                for i in range(number_of_triangles):
                    line = file.readline() # row no label x1 y1 z1 edges 
                    line = file.readline() # row x2 y2 z2
                    line = file.readline() # row x3 y3 z3
                    line = file.readline() # row nx ny nz area

            if "DATA OF THE METALLIC EDGES" in line:
                line = file.readline() # empty line
                line = file.readline() # first header
                line = file.readline() # second header

                for i in range(number_of_edges):
                    line = file.readline() # no type length KORP KORM

            



freader = FEKOFileReader("C:\\Users\\Tameez\\Documents\\git\\SUN-EM\\examples\\two_plate_array\\pec_plate.out")
freader.readFEKOOutFile()