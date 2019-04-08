#include "mom_file_reader.h"

/**
 *   \file mom_file_reader.cpp
 *   \brief Read the .mom file
 *
 *  Detailed description
 *  Read the .mom file and assign the information to all the relevant
 *  data structures.
 *
 *  Author:  Tameez Ebrahim
 *  Created: 25 July 2018
 *
 */

MoMFileReader::MoMFileReader(std::string file_path, bool cbfm)
{
    // Lets open the file
    std::ifstream file(file_path);

    // Lets create a string to use for the received lines
    std::string str;

    // Lets create an in to hold the number of fields
    int num_fields = 0;

    // Lets create a string vector to store key-value pairs from MoMFileReader::constLineReader
    // This will be used for Const entries and entry numbers
    std::vector<std::string> line_vector;

    if(file.is_open())
    {

        // Lets read first three lines
        // Basically header stuff
        getline(file, str);
        getline(file, str);
        getline(file, str);

        // Lets read Const
        getline(file, str);
        if(str == "CONST START")
        {
            // Lets get the number of fields
            getline(file, str);
            line_vector = this->constLineReader(str);
            num_fields = std::stoi(line_vector[1]);

            // Lets read the empty line
            getline(file, str);

            // Lets read all the Const entries now
            for(int i = 0; i < num_fields; i++)
            {
                getline(file, str);
                line_vector = this->constLineReader(str);

                // Now lets put the information from Const into the map (Dictionary) created in the class
                this->const_map[line_vector[0]] = line_vector[1];
            }
            //-------------------------------
            //TODO: FIX FOR MULTIPLE FREQS
            this->const_map["cppFreq"] = this->const_map["freq"];
            this->const_map["cppFreq"].erase(0,1);
            this->const_map["cppFreq"].pop_back();
            //-------------------------------

            //-------------------------------
            //TODO: 
            //-------------------------------
            
            // for(const auto &p : this->const_map)
            // {
            //     std::cout << p.first << " <-> " << p.second << std::endl;
            // }
        }
        else
        {
            std::cout << "ERROR: THERE IS SOMETHING WRONG WITH THE FILE" << std::endl;
            std::cout << "ERROR: CANNOT FIND CONST ENTRIES" << std::endl;
        }

        // Lets check that all Const entries are read
        getline(file, str);

        if(!(str == "CONST END"))
        {
            std::cout << str << std::endl;
            std::cout << "ERROR: THERE IS SOMETHING WRONG WITH THE FILE" << std::endl;
            std::cout << "ERROR: THE NUMBER OF CONST ENTRIES ARE WRONG" << std::endl;
        }

        // Lets start getting the FEKO_data
        // Lets first read the empty line
        getline(file, str);

        // Now lets check for FEKO_data
        getline(file, str);

        if(str == "FEKO_DATA START")
        {
            // Lets first read the nodes
            getline(file, str);

            if(str == "NODES START")
            {
                // Lets get the number of fields i.e the number of nodes
                getline(file, str);
                line_vector = this->constLineReader(str);
                num_fields = std::stoi(line_vector[1]);

                // Lets read the node header
                getline(file, str);

                // Now lets read the nodes
                for(int i = 0; i < num_fields; i++)
                {
                    getline(file, str);
                    line_vector = this->numberLineReader(str, 3);

                    // Lets add the values to the Node class and push it to node_vector
                    // Also have to change the string to float
                    this->nodes.push_back(Node<double>{ std::stod(line_vector[0]),
                                                        std::stod(line_vector[1]),
                                                        std::stod(line_vector[2])});
                }
            }
            else
            {
                std::cout << "ERROR: THERE IS SOMETHING WRONG WITH THE FILE" << std::endl;
                std::cout << "ERROR: CANNOT FIND NODES" << std::endl;
            }

            // Lets check if all the nodes have been read
            getline(file, str);

            if(!(str == "NODES END"))
            {
                std::cout << "ERROR: THERE IS SOMETHING WRONG WITH THE FILE" << std::endl;
                std::cout << "ERROR: THE NUMBER OF NODE ENTRIES ARE WRONG" << std::endl;
            }

            // Lets start reading the triangles
            // First read the empty line
            getline(file, str);

            // Lets check for the triangles
            getline(file, str);

            if(str == "TRIANGLES START")
            {
                std::vector<int> unique_labels;
                // First lets get the number of triangles
                getline(file, str);
                line_vector = this->constLineReader(str);
                num_fields = std::stoi(line_vector[1]);

                // Now lets read the empty line and the header
                getline(file, str);
                getline(file, str);

                // Now lets read the triangle data
                // The data is of the form VERTEX-VERTEX-VERTEX-CENTREX-CENTREY-CENTREZ-AREA
                // The vertices refer to the index of the nodes
                for(int i = 0; i < num_fields; i++)
                {
                    getline(file, str);

                    line_vector = this->numberLineReader(str, 8);

                    // Lets read to the triangle class
                    // Lets first make a node to store the centre
                    // Dont forget to convert from string
                    Node<double> centre_node{std::stod(line_vector[3]),
                            std::stod(line_vector[4]),
                            std::stod(line_vector[5])};

                    // Now lets read to a triangle
                    // Also, minus 1 from the vertex indices due to MATLAB starting from 1 and
                    // everything else starting from 0
                    Triangle triangle;
                    triangle.vertex_1 = std::stoi(line_vector[0]);
                    triangle.vertex_2 = std::stoi(line_vector[1]);
                    triangle.vertex_3 = std::stoi(line_vector[2]);
                    triangle.centre = centre_node;
                    triangle.area = std::stod(line_vector[6]);
                    triangle.label = std::stoi(line_vector[7]);
                    
                    //-------------------------------
                    //TODO: Come backe when changing labels to @
                    if(cbfm)
                    {
                        this->label_map[triangle.label].triangle_indices.push_back(i); 
                    }
                    else
                    {
                        this->label_map[0].triangle_indices.push_back(i);
                    }
                    //-------------------------------
                    
                    // Finally, lets push to vector
                    this->triangles.push_back(triangle);

                }
            }
            else
            {
                std::cout << "ERROR: THERE IS SOMETHING WRONG WITH THE FILE" << std::endl;
                std::cout << "ERROR: CANNOT FIND TRIANGLE DATA" << std::endl;
            }

            // Lets check that all the triangle data was read
            getline(file, str);
            if(!(str == "TRIANGLES END"))
            {
                std::cout << "ERROR: THERE IS SOMETHING WRONG WITH THE FILE" << std::endl;
                std::cout << "ERROR: THE NUMBER OF TRAINGLE ENTRIES ARE WRONG" << std::endl;
            }

            // Lets read the edge data
            // Start by reading the empty line
            getline(file, str);

            // Now lets check for the edges
            getline(file, str);

            if(str == "EDGES START")
            {
                // Lets get the number of edges
                getline(file, str);
                line_vector = this->constLineReader(str);
                num_fields = std::stoi(line_vector[1]);

                // Now lets read the empty line and the header
                getline(file, str);
                getline(file, str);

                // Lets read the edge data
                // The data is of the form VERTEX-VERTEX-CENTRE_X-CENTRE_Y-CENTRE_Z-LENGTH-TRIANGLE_MINUS-
                // TRIANGLE_PLUS-MINUS_FREE_VERTEX-PLUS_FREE_VERTEX-RHO_C_MINUS_X-RHO_C_MINUS_Y-
                // RHO_C_MINUS_Z-RHO_C_PLUS_X-RHO_C_PLUS_Y-RHO_C_PLUS_Z
                for(int i = 0; i < num_fields; i++)
                {
                    getline(file, str);

                    line_vector = this->numberLineReader(str, 16);

                    // Lets read the data into class Edge
                    // First, lets make the Nodes for the centre and the two rho's
                    // Don't forget to convert from string
                    Node<double> centre{std::stod(line_vector[2]),
                                        std::stod(line_vector[3]),
                                        std::stod(line_vector[4])};

                    Node<double> rho_c_minus{   std::stod(line_vector[10]),
                                                std::stod(line_vector[11]),
                                                std::stod(line_vector[12])};

                    Node<double> rho_c_plus{std::stod(line_vector[13]),
                                            std::stod(line_vector[14]),
                                            std::stod(line_vector[15])};

                    // Now lets add the data into an Edge
                    // Also, lets reduce all indices by 1 due to MATLAB starting from 1 and everything
                    // else starting from 0
                    Edge edge{std::stoi(line_vector[0]), // vertex_1
                            std::stoi(line_vector[1]), // vertex_2
                            centre,                        // center
                            std::stod(line_vector[5]),     // length
                            std::stoi(line_vector[6]), // minus_triangle_index
                            std::stoi(line_vector[7]), // plus_triangle_index
                            std::stoi(line_vector[8]), // minus_free_vertex
                            std::stoi(line_vector[9]), // plus_free_vertex
                            rho_c_minus,                   // rho_c_minus
                            rho_c_plus};                   // rhos_c_plus

                    // Lets introduce an index to the edge in the plus and minus triangles
                    // This is needed for the calculation of Zmn by face.
                    this->triangles[std::stoi(line_vector[6])].edge_indices.push_back(i);
                    this->triangles[std::stoi(line_vector[7])].edge_indices.push_back(i);
                    
                    //-------------------------------
                    //TODO: Revisit
                    // Add number of edges per label
                    // Only use positive triangles so as not to double up
                    if(cbfm)
                    {
                        this->label_map[this->triangles[std::stoi(line_vector[6])].label].edge_indices.push_back(i);
                    }
                    else
                    {
                        this->label_map[0].edge_indices.push_back(i);
                    }
                    //-------------------------------

                    this->edges.push_back(edge);
                }
            }
            else
            {
                std::cout << "ERROR: THERE IS SOMETHING WRONG WITH THE FILE" << std::endl;
                std::cout << "ERROR: CANNOT FIND EDGE DATA" << std::endl;
            }

            // Lets check that all the edges were read
            getline(file, str);

            if(!(str == "EDGES END"))
            {
                std::cout << "ERROR: THERE IS SOMETHING WRONG WITH THE FILE" << std::endl;
                std::cout << "ERROR: THE NUMBER OF TRAINGLE ENTRIES ARE WRONG" << std::endl;
            }
        }
        else
        {
            std::cout << "ERROR: THERE IS SOMETHING WRONG WITH THE FILE" << std::endl;
            std::cout << "ERROR: CANNOT FIND FEKO_DATA" << std::endl;
        }

        // Lets check that all the FEKO_data was read
        getline(file, str);

        if(!(str == "FEKO_DATA END"))
        {
            std::cout << "ERROR: THERE IS SOMETHING WRONG WITH THE FILE" << std::endl;
            std::cout << "ERROR: FEKO_DATA HAS NOT ENDED" << std::endl;
        }      

        // Read excitations
        getline(file, str);
        getline(file, str);
        if (str == "EXCITATIONS START")
        {
            getline(file, str);
            line_vector = this->constLineReader(str);
            num_fields = std::stoi(line_vector[1]);

            getline(file, str); // read empty line

            for (int i = 0; i < num_fields; i++)
            {
                Excitation tmp_excitation;
                getline(file, str); // index

                getline(file, str); // type
                line_vector = this->constLineReader(str);
                tmp_excitation.type = std::stoi(line_vector[1]);
                
                getline(file, str); // label
                line_vector = this->constLineReader(str);
                tmp_excitation.label = std::stoi(line_vector[1]);

                getline(file, str); // emag
                line_vector = this->constLineReader(str);
                tmp_excitation.emag = std::stod(line_vector[1]);

                if (tmp_excitation.type == 1)
                {
                    getline(file, str); // theta
                    line_vector = this->constLineReader(str);
                    tmp_excitation.theta = std::stod(line_vector[1]);
                    
                    getline(file, str); // phi
                    line_vector = this->constLineReader(str);
                    tmp_excitation.phi = std::stod(line_vector[1]);

                }

                if (tmp_excitation.type == 2)
                {
                    getline(file, str); // numPorts
                    line_vector = this->constLineReader(str);
                    int num_ports = std::stoi(line_vector[1]);

                    for (int j = 0; j < num_ports; j++)
                    {
                        getline(file, str); // ports
                        line_vector = this->constLineReader(str);
                        tmp_excitation.ports.push_back(std::stoi(line_vector[1]));
                    }
                }
                
                this->excitations.push_back(tmp_excitation);
                
                getline(file, str); // read empty line
            }
            
        }
        // TODO: Add in end check for excitations
        
    }
    else
    {
        std::cout << "ERROR: THE FILE CANNOT BE OPENED" << std::endl;
    }
}

std::map<std::string, std::string> MoMFileReader::getConstMap()
{
    // Return the map of all the Const data
    return this->const_map;
}

std::vector<Node<double>> MoMFileReader::getNodes()
{
    return this->nodes;
}

std::vector<Triangle> MoMFileReader::getTriangles()
{
    return this->triangles;
}

std::vector<Edge> MoMFileReader::getEdges()
{
    return this->edges;
}

std::vector<std::string> MoMFileReader::constLineReader(std::string line)
{
    // line is of the form
    // KEY..........--VALUE
    // The .'s are spaces and the -'s are tabs
    // The file is tab delimited but the spaces are introduced -
    // by matlab due to formatting reasons


    // Lets get the index till the first space
    // Then cut the string till the first space to get the starting word
    int first_whitespace_char_index = std::strcspn(line.c_str(), " ");
    std::string key = line.substr(0, first_whitespace_char_index);

    // Lets reverse the line
    // Then lets get the index of the first tab
    // It looks like eulav--....yek
    std::reverse(line.begin(), line.end());
    int last_whitespace_char_index = line.length() - std::strcspn(line.c_str(), "\t");

    // Lets then reverse back to normal to cut
    // Lets cut from the back till the index
    std::reverse(line.begin(), line.end());
    std::string value = line.substr(last_whitespace_char_index, line.length() - 1);

    // Lets return the values as a string vector
    std::vector<std::string> key_value_vector;
    key_value_vector.push_back(key);
    key_value_vector.push_back(value);
    return key_value_vector;
}

std::vector<std::string> MoMFileReader::numberLineReader(std::string line, int num_values)
{
    // line is of the form
    // VALUE-VALUE-VALUE etc.
    // The -'s are tabs
    // num_values provides the number of values


    // Lets create a vector to return the values
    std::vector<std::string> value_vector;

    // Lets get the values with a tab find
    // Then lets add them to value_vector
    // Let us first create an index
    int index = 0;

    // The loop needs to be one less than the values because we don't need to erase the last one
    for(int i = 0; i < num_values - 1; i++)
    {
        // Lets find the first tab
        index = line.find('\t');

        // Now lets add the string till the first tab
        value_vector.push_back(line.substr(0, index));

        // Then lets erase the the added value as well as the tab
        line.erase(0, index + 1);
    }

    // Lets add the last value
    value_vector.push_back(line);

    // Lets return the values
    return value_vector;
}
