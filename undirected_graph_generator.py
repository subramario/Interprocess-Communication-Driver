import sys
import re
import numpy

# This function checks whether intersection point(s) exists between two lines
def checks(x1,y1,x2,y2,x3,y3,x4,y4):
    try:
        # t and u are part of the line intersection equation
        t_numerator = ( ((x1-x3)*(y3-y4)) - ((y1-y3)*(x3-x4)) )
        t_denominator = ( ((x1-x2)*(y3-y4)) - ((y1-y2)*(x3-x4)) )

        u_numerator = ( ((x1-x2)*(y1-y3)) - ((y1-y2)*(x1-x3)) )
        u_denominator = ( ((x1-x2)*(y3-y4)) - ((y1-y2)*(x3-x4)) )

        t = t_numerator/t_denominator
        u = -u_numerator/u_denominator

        #u and t are out of bounds --> distinct lines, not overlapping
        if (u < 0 or u > 1 or t < 0 or t > 1): 
            return 0

        # Executes if only one intersection point exists between two lines 
        if ((0 <= u <= 1) and (0 <= t <= 1)):
            x_coord = x1 + (t * (x2 - x1))
            y_coord = y1 + (t * (y2 - y1))
            return [(float("{0:.2f}".format(x_coord)),float("{0:.2f}".format(y_coord)))]
    
    except:
        # These statements describe overlapping line segment scenarios for all orientations and returns both intersection points
        if (t_numerator == 0 and t_denominator == 0 and u_numerator == 0 and u_denominator == 0): 
            if ((x3 <= x1 <= x4 and x3 <= x2 <= x4 and y3 <= y1 <= y4 and y3 <= y2 <= y4) or
                (x4 <= x1 <= x3 and x4 <= x2 <= x3 and y4 <= y1 <= y3 and y4 <= y2 <= y3) or
                (x3 <= x1 <= x4 and x3 <= x2 <= x4 and y3 >= y1 >= y4 and y3 >= y2 >= y4) or
                (x4 <= x1 <= x3 and x4 <= x2 <= x3 and y4 >= y1 >= y3 and y4 >= y2 >= y3)):
                return [(x1, y1), (x2, y2)]

            if ((x2 <= x3 <= x1 and x2 <= x4 <= x1 and y2 <= y3 <= y1 and y2 <= y4 <= y1) or
                (x1 <= x3 <= x2 and x1 <= x4 <= x2 and y1 <= y3 <= y2 and y1 <= y4 <= y2) or
                (x2 <= x3 <= x1 and x2 <= x4 <= x1 and y2 >= y3 >= y1 and y2 >= y4 >= y1) or
                (x1 <= x3 <= x2 and x1 <= x4 <= x2 and y1 >= y3 >= y2 and y1 >= y4 >= y2)):
                return [(x3, y3), (x4, y4)]

            if ((x1 <= x3 <= x2 <= x4 and y1 >= y3 >= y2 >= y4) or
                (x4 <= x2 <= x3 <= x1 and y4 >= y2 >= y3 >= y1) or
                (x1 >= x3 >= x2 >= x4 and y1 >= y3 >= y2 >= y4) or
                (x4 >= x2 >= x3 >= x1 and y4 >= y2 >= y3 >= y1)):
                return [(x3, y3), (x2, y2)]

            if ((x1 <= x4 <= x2 <= x3 and y1 >= y4 >= y2 >= y3) or
                (x3 <= x2 <= x4 <= x1 and y3 >= y2 >= y4 >= y1) or
                (x1 >= x4 >= x2 >= x3 and y1 >= y4 >= y2 >= y3) or
                (x3 >= x2 >= x4 >= x1 and y3 >= y2 >= y4 >= y1)):
                return [(x2, y2), (x4, y4)]
            
            if ((x2 <= x4 <= x1 <= x3 and y2 >= y4 >= y1 >= y3) or
                (x3 <= x1 <= x4 <= x2 and y3 >= y1 >= y4 >= y2) or
                (x2 >= x4 >= x1 >= x3 and y2 >= y4 >= y1 >= y3) or
                (x3 >= x1 >= x4 >= x2 and y3 >= y1 >= y4 >= y2)):
                return [(x1, y1), (x4, y4)]

            if ((x4 <= x1 <= x3 <= x2 and y4 >= y1 >= y3 >= y2) or
                (x2 <= x3 <= x1 <= x4 and y2 >= y3 >= y1 >= y4) or
                (x4 >= x1 >= x3 >= x2 and y4 >= y1 >= y3 >= y2) or
                (x2 >= x3 >= x1 >= x4 and y2 >= y3 >= y1 >= y4)):
                return [(x1, y1), (x3, y3)]

        # The lines are not overlapping --> parallel and not touching, no intersection
        if (t_denominator == 0 and u_denominator == 0):  
            return 0

    
    return 0

# Calculates the distance between two points
def distance(refx, refy, intx, inty):
    dist = sqrt((intx - refx)**2 + (inty - refy)**2)
    return dist

def main():

    # Regular expressions which govern an acceptable input format for each command: a,c,r,g
    a = re.compile(r'^\s*[a]\s+["][a-zA-Z\s]+\s*["]\s+([(]\s*-?[0-9]+\s*[,]\s*-?[0-9]+\s*[)]\s*){2,}\s*$')
    c = re.compile(r'^\s*[c]\s+["][a-zA-Z\s]+\s*["]\s+([(]\s*-?[0-9]+\s*[,]\s*-?[0-9]+\s*[)]\s*){2,}\s*$')
    r = re.compile(r'^\s*[r]\s+["][a-zA-Z\s]+\s*["]\s*$')
    g = re.compile(r'^\s*[g]\s*$')

    # Dictionary which stores all street names and corresponding poly-line segment coordinates in string format
    street_info = {}

    try:
        while True:
            prompt = input()

            # Simple counter to keep track of the number of entries in the dictionary
            count = 0
            for key in street_info:
                count += 1

            # Parse user input and store the street name along with all coordinates into variables "street" and "coord" respectively
            street = re.search('["][a-zA-Z\s]+\s*["]', prompt)
            coord = re.search('([(]\s*-?[0-9]+\s*[,]\s*-?[0-9]+\s*[)]\s*){2,}\s*', prompt)


            # Each if statement contains procedures related to each command type -> this entire if-else block acts as a switch statement 

            # a = adding a new street
            if a.fullmatch(prompt):
                # Next 2 lines format the street name and coordinates appropriately 
                street_key = street.group().lower() 
                street_value = coord.group().replace(" ", "").replace(")", ".0)").replace(",", ".0,").replace(")(", ") (").split(" ")

                # Checks if street name already exists in the dictionary before adding it (no duplicate streets allowed)
                if street_key in street_info:
                    print("Error: Street name already exists in the system, no duplicate entries allowed.")
                else:
                    street_info[street_key] = street_value
                    count += 1

            # c = change the coordiantes of an existing street
            elif c.fullmatch(prompt):
                if count == 0:
                    print("Error: Cannot change specified street when no street has been added to the system.")
                else:
                    street_key = street.group().lower() 
                    street_value = coord.group().replace(" ", "").replace(")", ".0)").replace(",", ".0,").replace(")(",") (").split(" ")
                
                    # Replaces coordinates for the street that is changed
                    if street_key in street_info:
                        street_info[street_key] = street_value
                    else:
                        print("Error: 'c' specified for a street that does not exist.")
                        
            # r = remove an existing street
            elif r.fullmatch(prompt):
                if count == 0:
                    print("Error: System does not have any entries to remove.")
                else:
                    street_key = street.group()
                    if street_key in street_info:
                        del street_info[street_key]
                        count -= 1
                    else:
                        print("Error: 'r' specified for a street that does not exist.")
                    
            # g = graph the street map to produce an undirected graph
            elif g.fullmatch(prompt):

                # Assesses whether any streets have been added to the system
                check = bool(street_info)

                if check == False:
                    print("Error: Cannot issue graphing command as no streets have been added.")
                else:
                    # Copy of street_info dictionary - except all coordinates are type-casted to Tuples!
                    converted = {}
                    for key,value in street_info.items():
                        converted[str(key)] = [eval(item) for item in value]

                    # Stores all the vertices in the street map
                    vertices = []

                    # Stores unsorted, redundant edge list here --> will be refined later
                    temp_edge_list = []

                    # This loop (lines 148-231) finds all valid intersection points in the street map and orders them with respect to distance
                    for key_temp in converted:
                        
                        # Stores all intersection points in the street map as keys, and stores all line segment endpoints which make these intersection as values
                        ip_dict = {}

                        # Stores the coordinates which make up a street as an array of tuples
                        value_curr = converted[key_temp]

                        # This loop (lines 156-184) finds all intersection points within the street map and stores it into "ip_dict" dictionary
                        for i in range(0, len(value_curr) - 1):
                            # Extract coordinates for first line segment 
                            x1, y1 = value_curr[i][0], value_curr[i][1]
                            x2, y2 = value_curr[i + 1][0], value_curr[i + 1][1]
                            
                            for key in converted:

                                if key != key_temp:
                                    other_street_val = converted[key]

                                    for k in range(0, len(other_street_val) - 1):
                                        # Extract coordinates for second line segment 
                                        x3, y3 = other_street_val[k][0], other_street_val[k][1]
                                        x4, y4 = other_street_val[k + 1][0], other_street_val[k + 1][1]

                                        # Function "checks" evaluates whether intersection point(s) exist between both line segments in comparison 
                                        ip = checks(x1,y1,x2,y2,x3,y3,x4,y4)

                                        if ip != 0:
                                            for pt in ip:
                                                if pt in ip_dict:
                                                    val = ip_dict[pt]
                                                    val.append((x1, y1))
                                                    val.append((x2, y2))
                                                    val.append((x3, y3))
                                                    val.append((x4, y4))
                                                else:
                                                    ip_dict[pt] = [(x1, y1),(x2, y2),(x3, y3),(x4, y4)]

                        
                        # This loop (lines 188-231) sorts all the intersection points in order by distance - thereby producing a correct edge list for the undirected graph - and creates the edge list
                        for x in range(0, len(value_curr) - 1):
                            
                            temp = []

                            p_x1, p_y1 = value_curr[x][0], value_curr[x][1]
                            p_x2, p_y2 = value_curr[x + 1][0], value_curr[x + 1][1]

                            for key_ip in ip_dict:

                                val = ip_dict[key_ip] 
                            
                                for j in range(0,len(val) - 1):

                                    p1, q1 = val[j][0], val[j][1]
                                    p2, q2 = val[j+1][0], val[j+1][1]
                                    
                                    if p_x1 == p1 and p_y1 == q1 and p_x2 == p2 and p_y2 == q2:
                                        if key_ip not in temp:
                                            temp.append(key_ip) 
                                        break
                                    j = j + 1

                            if len(temp) > 1:
                                Ap = numpy.array([p_x1, p_y1])  
                                B = numpy.asarray(temp)  
                                dist = numpy.linalg.norm(B - Ap, ord=2,axis=1)  
                                sorted_B = B[numpy.argsort(dist)]
                    
                                temp = sorted_B.tolist()
                                temp = [tuple(y) for y in temp]

                            if len(temp) >= 1:
                                temp.insert(0,(p_x1, p_y1))
                                temp.append((p_x2, p_y2))

                            final_list = []
                            for intersection in temp:
                                if intersection not in final_list:
                                    final_list.append(intersection)
                        
                            if len(final_list) >= 2:
                                for j in range(0,len(final_list)-1):
                                    temp_edge_list.append((final_list[j], final_list[j+1]))
                                    
                    redundant_edge_list = []        
                    for item in temp_edge_list:
                        if item not in redundant_edge_list:
                            redundant_edge_list.append(item)

                    # Checks for and removes any duplicate edges from the edge list
                    final_edge_list = []
                    for y in range(0, len(redundant_edge_list)):
                        duplicate_1 = (redundant_edge_list[y][0], redundant_edge_list[y][1])
                        duplicate_2 = (redundant_edge_list[y][1], redundant_edge_list[y][0])
                        if (duplicate_1 not in final_edge_list) and (duplicate_2 not in final_edge_list):
                            final_edge_list.append(duplicate_1)
                    
                    # Now that the final edge list is complete, this portion creates the vertice list from the edge list
                    for x in range(0, len(final_edge_list)):

                        if final_edge_list[x][0] not in vertices:
                            vertices.append(final_edge_list[x][0])

                        if final_edge_list[x][1] not in vertices:
                            vertices.append(final_edge_list[x][1])


                    # Prints out the undirected graphs vertice and edge list!
                    print("V", len(vertices))

                    if len(final_edge_list) == 0:
                        print("E {}")
                        
                    elif len(final_edge_list) == 1:
                        x = vertices.index(final_edge_list[0][0])
                        y = vertices.index(final_edge_list[0][1])
                        print("E {<",x,",",y,">}", sep = "")
                        
                        
                    else:
                        print("E {", end = '')
                        for j in range(0, len(final_edge_list)-1):
                            x = vertices.index(final_edge_list[j][0])
                            y = vertices.index(final_edge_list[j][1])
                            print("<",x,",",y,">,", sep = "", end = '')
                        j += 1
                        x = vertices.index(final_edge_list[j][0])
                        y = vertices.index(final_edge_list[j][1])
                        print("<",x,",",y,">}\n", sep = "", end = '')

                    
            else:
                print("Error: Invalid entry. Please adhere to the specified format and enter command again.")
    
    except EOFError:
        return

if __name__ == '__main__':
    main()