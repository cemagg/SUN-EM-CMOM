class Node:

    def __init__(self, x_coord, y_coord, z_coord):
        self.x = x_coord
        self.y = y_coord
        self.z = z_coord

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y and self.z == other.z

    def __add__(self, other):
        return Node(self.x + other.x, self.y + other.y, self.z + other.z)

    def __sub__(self, other):
        return Node(self.x - other.x, self.y - other.y, self.z - other.z)

    def __mul__(self, other):
        return Node(other * self.x, other * self.y, other * self.z)
    
    def __rmul__(self, other):
        return Node(other * self.x, other * self.y, other * self.z)


class Triangle:

    def __init__(self, vertex_1, vertex_2, vertex_3, centre, area, label):
        self.v1 = vertex_1
        self.v2 = vertex_2
        self.v3 = vertex_3
        self.centre = centre
        self.area = area
        self.label = label

class Edge:

    def __init__(self, vertex_1, vertex_2, centre, length, t_minus, t_plus, m_free_vertex, p_free_vertex, rho_c_minus, rho_c_plus):
        self.v1 = vertex_1
        self.v2 = vertex_2
        self.centre = centre
        self.length = length
        self.tm = t_minus
        self.tp = t_plus
        self.mf = m_free_vertex
        self.pf  = p_free_vertex
        self.rcm = rho_c_minus
        self.rcp = rho_c_plus

class Excitation:
    def __init__(self, index):
        self.index = index
        self.type = 0

    def planeWave(self, theta, phi, emag):
        self.type = 1        
        self.theta = theta
        self.phi = phi
        self.emag = emag

    def edgePort(self, ports, emag):
        self.type = 2
        self.ports = ports
        self.emag = emag


def triangleCentreCalculator(vertices):
    return (1/3) * (vertices[0] + vertices[1] + vertices[2])

def getEdgeVertices(plus_triangle, minus_triangle):
    edge_vertices = []

    if plus_triangle.v1 == minus_triangle.v1 or plus_triangle.v1 == minus_triangle.v2 or plus_triangle.v1 == minus_triangle.v3:
        edge_vertices.append(plus_triangle.v1)
    
    if plus_triangle.v2 == minus_triangle.v1 or plus_triangle.v2 == minus_triangle.v2 or plus_triangle.v2 == minus_triangle.v3:
        edge_vertices.append(plus_triangle.v2)

    if plus_triangle.v3 == minus_triangle.v1 or plus_triangle.v3 == minus_triangle.v2 or plus_triangle.v3 == minus_triangle.v3:
        edge_vertices.append(plus_triangle.v3)
    
    return edge_vertices

def edgeCentreCalculator(vertex_1, vertex_2):
    return Node((vertex_1.x + vertex_2.x)/2, (vertex_1.y + vertex_2.y)/2, (vertex_1.z + vertex_2.z)/2)

def egdeGetFreeVertex(triangle, vertices):
    if (triangle.v1 == vertices[0] or triangle.v1 == vertices[1]) and (triangle.v2 == vertices[0] or triangle.v2 == vertices[1]):
        return triangle.v3
    elif (triangle.v1 == vertices[0] or triangle.v1 == vertices[1]) and (triangle.v3 == vertices[0] or triangle.v3 == vertices[1]):
        return triangle.v2
    else:
        return triangle.v1

def edgeRhoPlusMinusCalculator(triangle, free_vertex, plus_or_minus):
    if plus_or_minus == 0: # get rho_c_plus
        return triangle.centre - free_vertex
    else:
        return -1 * (triangle.centre - free_vertex)


