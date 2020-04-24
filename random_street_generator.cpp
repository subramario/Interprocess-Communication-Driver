#include <iostream>
#include <unistd.h>
#include <fstream>
#include <tuple>
#include <vector>
#include <typeinfo>
#include <iterator> 
#include <map>
#include <getopt.h>

using namespace std;

int ip_count = 0; //must be greater than one for graph to be valid



void print_graph(const map<string,vector<tuple<int,int>>> &verified_streets) {
    for (auto const& l : verified_streets){
                
        string name = l.first;  

        cerr << name << " ";   
        unsigned int r;       
        vector < tuple<int,int> > meow = l.second; //Returns vector of tuples for each street
        for (r = 0; r < meow.size(); r++){
            int g = get<0>(meow[r]);
            int h = get<1>(meow[r]);

            cerr << "(" << g << "," << h << ") ";

        }
        cerr << endl;
    }

}


bool zero_length_check(int x1, int y1, int x2, int y2) {

    if (x1 == x2 && y1 == y2)
        return true;
    else
        return false;
}

bool other_street_intersection_check(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    float t_numerator = ((x1-x3)*(y3-y4)) - ((y1-y3)*(x3-x4));
    float t_denominator = ((x1-x2)*(y3-y4)) - ((y1-y2)*(x3-x4));
    
    float u_numerator = -(((x1-x2)*(y1-y3)) - ((y1-y2)*(x1-x3)));
    float u_denominator = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4));

    float t = (t_numerator)/(t_denominator);
    float u = (u_numerator)/(u_denominator);

    if ((0.0 <= u) && (u <= 1.0) && (0.0 <= t) && (t <= 1.0)) //This means there is an intersection
        return true;
    else
        return false;
}

bool same_street_intersection_check(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {

    float t_numerator = ((x1-x3)*(y3-y4)) - ((y1-y3)*(x3-x4));
    float t_denominator = ((x1-x2)*(y3-y4)) - ((y1-y2)*(x3-x4));
    

    float u_numerator = -(((x1-x2)*(y1-y3)) - ((y1-y2)*(x1-x3)));
    float u_denominator = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4));


    if (t_numerator == 0.0 && u_numerator == 0.0 && t_denominator == 0.0 && u_denominator == 0.0) //If both numerator and denominator for both u and t are zero, then lines are overlapping
        return true;

    if (t_denominator == 0.0 && u_denominator == 0.0) //This means that lines are parallel but not touching
        return false;


    float t = (t_numerator)/(t_denominator);
    float u = (u_numerator)/(u_denominator);

    if ((0.0 <= u) && (u <= 1.0) && (0.0 <= t) && (t <= 1.0)) //This means there is an intersection
        return true;
        // float x_coord = x1 + (t * (x2 - x1));
        // float y_coord = y1 + (t * (y2 - y1));

    else
        return false; //This means the lines aren't parallel nor are they intersecting 

    // cout << "Same street intersection check: " << t << " " << u << endl;
}

bool same_street_special_check(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {

    float t_numerator = ((x1-x3)*(y3-y4)) - ((y1-y3)*(x3-x4));
    float t_denominator = ((x1-x2)*(y3-y4)) - ((y1-y2)*(x3-x4));

    float u_numerator = -(((x1-x2)*(y1-y3)) - ((y1-y2)*(x1-x3)));
    float u_denominator = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4));

    if (t_numerator == 0.0 && u_numerator == 0.0 && t_denominator == 0.0 && u_denominator == 0.0) //If both numerator and denominator for both u and t are zero, then lines are overlapping
        return true;
    else
        return false;
}

bool other_street_overlap_check(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {

    float t_numerator = ((x1-x3)*(y3-y4)) - ((y1-y3)*(x3-x4));
    float t_denominator = ((x1-x2)*(y3-y4)) - ((y1-y2)*(x3-x4));
    

    float u_numerator = -(((x1-x2)*(y1-y3)) - ((y1-y2)*(x1-x3)));
    float u_denominator = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4));

    if (t_numerator == 0.0 && u_numerator == 0.0 && t_denominator == 0.0 && u_denominator == 0.0) //If both numerator and denominator for both u and t are zero, then lines are overlapping
        return true;
    return false;
}

bool validity(int x1, int y1, int x2, int y2, map<string,vector<tuple<int,int>>> verified_streets, string street_name) {
    // cout << "YO IM IN" << endl;
    // bool current_check = true;
    // bool street_overlap_check = true;


    if (zero_length_check( x1, y1, x2, y2) == true){
        // cout << "failed here" << endl;
        return false;
    }

     vector<tuple<int,int>> value= verified_streets[street_name];


    for(unsigned int i = 0; i < value.size()-1; i++) {
        int x3 = get<0>(value[i]);
        int y3 = get<1>(value[i]);
        int x4 = get<0>(value[i+1]);
        int y4 = get<1>(value[i+1]);

        if (i == (value.size()-2)){
            if (same_street_special_check(x1,y1, x2, y2, x3, y3, x4, y4) == true){
                // cout << "meow" << endl;
                return false;
            }
        }
        else {
            if (same_street_intersection_check(x1,y1, x2, y2, x3, y3, x4, y4) == true)
                return false;
            // else
                // current_check = true;
        }
    }

    for (auto const& d : verified_streets){
        string bark = d.first;                
        if (bark != street_name){
            vector < tuple<int,int> > woof = d.second; //Returns vector of tuples for each street

            for (unsigned int r = 0; r < woof.size()-1; r++){
                int g = get<0>(woof[r]);
                int h = get<1>(woof[r]);
                int j = get<0>(woof[r+1]);
                int k = get<1>(woof[r+1]);
                // cout << g << endl;
                // cout << h << endl;
                // cout << j << endl;
                // cout << k << endl;

                if (other_street_overlap_check(x1,y1,x2,y2,g,h,j,k) == true){
                    // cout << "woof" << endl;
                    return false; 
                }
                
                if (other_street_intersection_check(x1,y1,x2,y2,g,h,j,k) == true) {//POTENTIAL FAILURE POINT
                    // cerr << "Checking other street intersection" << endl;
                    ip_count = ip_count + 1;
                }
                // else 
                //     return false;
                

                // street_overlap_check = true;               
            }
        }
    }

    // if (current_check == true && street_overlap_check == true) {
    //     // cout << "TRUE" << endl;
    return true;
    // }
    // else
    //     return false;
}
bool null_check(int x1, int y1, int x2, int y2, map<string,vector<tuple<int,int>>> verified_streets, string street_name) {
    // cerr << "made it into null_check" << endl;
    for (auto const& d : verified_streets){
        string bark = d.first;                
        if (bark != street_name){
            vector < tuple<int,int> > woof = d.second; //Returns vector of tuples for each street

            for (unsigned int r = 0; r < woof.size()-1; r++){
                int g = get<0>(woof[r]);
                int h = get<1>(woof[r]);
                int j = get<0>(woof[r+1]);
                int k = get<1>(woof[r+1]);
                
                if (other_street_intersection_check(x1,y1,x2,y2,g,h,j,k) == true) //POTENTIAL FAILURE POINT
                    return true;

                    // ip_count = ip_count + 1;
            }
        }
    }

    return false;
}

int main (int argc, char **argv){

    //Default k values for each command line argument 
    int streets_k = 10;
    int segments_k = 5;
    int wait_k = 5;
    int coord_urange_k = 20;
    int coord_lrange_k = -20;
    int c;

    ifstream urandom("/dev/urandom");
    if (urandom.fail()) { 
        cerr << "Error: cannot open /dev/urandom\n";
        exit(0);
        return 1;
    }   

    opterr = 0;
 
    while ((c = getopt (argc, argv, "s:n:l:c:")) != -1){
        switch (c){
            case 's':
                if (atoi(optarg) == 0){
                    cerr << "Error: Invalid command - must include a valid positive integer with each command." << endl;
                    return 0;
                }

                if (atoi(optarg) < 2){
                    cerr << "Error: -s must be an integer greater than or equal to 2." << endl;
                    return 0;
                }
                else{
                    streets_k = atoi(optarg);
                    break;
                }
            case 'n':
                if (atoi(optarg) == 0){
                    cerr << "Error: Invalid command - must include a valid positive integer with each command." << endl;
                    return 0;
                }

                if (atoi(optarg) < 1){
                    cerr << "Error: -n must be an integer greater than or equal to 1." << endl;
                    return 0;
                }
                else{
                    segments_k = atoi(optarg);
                    break;
                }
            case 'l':
                 if (atoi(optarg) == 0){
                    cerr << "Error: Invalid command - must include a valid positive integer with each command." << endl;
                    return 0;
                }

                if (atoi(optarg) < 5){
                    cerr << "Error: -l must be an integer greater than or equal to 5." << endl;
                    return 0;
                }
                else{
                    wait_k = atoi(optarg);
                    break;
                }
            case 'c':
                if (atoi(optarg) == 0){
                    cerr << "Error: Invalid command - must include a valid positive integer with each command." << endl;
                    return 0;
                }

                if (atoi(optarg) < 1){
                    cerr << "Error: -c must be an integer greater than or equal to 1." << endl;
                    return 0;
                }
                else{
                    coord_urange_k = atoi(optarg);
                    coord_lrange_k = 0-coord_urange_k;
                    break;
                }
            case '?':
                if ((optopt == 's') || (optopt == 'n') || (optopt == 'l') || (optopt == 'c')){
                    cerr << "Error: option requires an argument." << endl;
                    exit(0);
                }
                else{
                    cerr << "Error: invalid option specified." << endl;
                    exit(0);
                }
                return 1;

            default:
                return 0;     
        }
    }

    // //Print statements used for troubleshooting
    // cout << "Streets: " << streets_k << endl;
    // cout << "Segments: " << segments_k << endl;
    // cout << "wait Time: " << wait_k << endl;
    // cout << "coordinate range: " << coord_urange_k << endl;
    // cout << endl;

    map<string,vector<tuple<int,int>>> verified_streets;
    vector <string> final; //Each index is a string which contains an add street command
    unsigned int num = 20; // read a random unsigned int


    // cout << "Streets_Random (2): " << streets << endl;
    // cout << endl;

    //GENERATE COORDINATE RANGE BEFORE LOOP

    //For each street --> generate line segments, coordinates for each line segment
    while(true) {

        if (verified_streets.size()!=0){
            // cerr << "Clearing Streets" << endl;
            for (auto const & d : verified_streets){
                string n = d.first;
                cout << "r " << "\"" << n << "\""<< std::endl;
            }

            verified_streets.clear();
        } 

        // Determines the random number of streets generated
        urandom.read((char*)&num, sizeof(num));
        int streets = num%(streets_k - 2 + 1) + 2;

        int count = 0;

        string street_name = "Z";

        // cerr << "Creating Streets" << endl;
        for (int i = 0; i < streets; i++){
            // vector < tuple<int,int> > v; //Used to store all coordinate points for current street in tuple format!

            urandom.read((char*)&num, sizeof(num));
            int segments = num%(segments_k - 1 + 1) + 1;

            //Created first point
            urandom.read((char*)&num, sizeof(num));
            int x1 = (num%(coord_urange_k - coord_lrange_k + 1)) + coord_lrange_k;

            urandom.read((char*)&num, sizeof(num));
            int y1 = (num%(coord_urange_k - coord_lrange_k + 1)) + coord_lrange_k;

            tuple <int, int> coord_1;
            coord_1 = make_tuple(x1, y1);
            verified_streets[street_name].push_back(coord_1);

            //For each line segment of one street, generate unique coordinates
            for (int j = 0; j < segments; j++){

                //Created second point
                urandom.read((char*)&num, sizeof(num));
                int x2 = (num%(coord_urange_k - coord_lrange_k + 1)) + coord_lrange_k;
                // cout << "X2_Random (1): " << x2 << "\n";

                urandom.read((char*)&num, sizeof(num));
                int y2 = (num%(coord_urange_k - coord_lrange_k + 1)) + coord_lrange_k;
                

                while (validity(x1,y1,x2,y2,verified_streets,street_name) == false && count < 25) {
                    // cerr << "Made it here" << endl;
                    urandom.read((char*)&num, sizeof(num));
                    x2 = (num%(coord_urange_k - coord_lrange_k + 1)) + coord_lrange_k;
                    // cout << "X2_Random (1): " << x2 << "\n";

                    urandom.read((char*)&num, sizeof(num));
                    y2 = (num%(coord_urange_k - coord_lrange_k + 1)) + coord_lrange_k;
                    // cout << "Y2_Random (1): " << y2 << "\n";

                    count += 1;

                }


                if (count == 25){   
                    cerr << "Error: Failed to generate valid input for 25 simultaneous attempts." << endl;
                    exit(0);
                }
                else {
                    tuple <int, int> coord_2;
                    coord_2 = make_tuple(x2, y2);
                    verified_streets[street_name].push_back(coord_2);
                    // cerr << "New Graph: "<< endl;
                    // print_graph(verified_streets);
                    count = 0;
                    //ip_count = 0;

                    x1 = x2;
                    y1 = y2;
                }
                
            }

            // if (i == streets-1){
            //     if (ip_count == 0){
            //         cerr << "NULL GRAPH GENERATED" << endl;
            //         count_1 = count_1 + 1;
            //         cerr << "Null graph tries: " << count_1 << endl;
            //         cerr << endl;
            //         i = 0;
            //         count_2 = 0;
            //         verified_streets.clear();
            //         continue;
            //     }


            // }
         
            //string coord_list(""); //Append string of tuples for each street to this string 
            
            street_name = street_name + "Z";
        }

        if (ip_count == 0){
            // cerr << "FOUND NO INTERSECTION" << endl;

            // print_graph(verified_streets);

            map<string,vector<tuple<int,int>>>::reverse_iterator rit;

            rit = verified_streets.rbegin();
            string last_name = rit->first;
            // tuple<int,int> last_point = rit->second;

            // cerr << "LAST STREET - " << last_name << endl;

            verified_streets[last_name].pop_back();
            tuple<int,int> last_tuple = verified_streets[last_name].back();

            count = 0;
            int x2, y2, x1, y1;

            while(ip_count == 0 && count < 25){
                urandom.read((char*)&num, sizeof(num));
                x2 = (num%(coord_urange_k - coord_lrange_k + 1)) + coord_lrange_k;
                // cout << "X2_Random (1): " << x2 << "\n";

                urandom.read((char*)&num, sizeof(num));
                y2 = (num%(coord_urange_k - coord_lrange_k + 1)) + coord_lrange_k;

                x1 = get<0>(last_tuple);
                y1 = get<1>(last_tuple);

                while(validity(x2,y2,x1,y1,verified_streets,last_name) == false){

                    urandom.read((char*)&num, sizeof(num));
                    x2 = (num%(coord_urange_k - coord_lrange_k + 1)) + coord_lrange_k;
                    // cout << "X2_Random (1): " << x2 << "\n";

                    urandom.read((char*)&num, sizeof(num));
                    y2 = (num%(coord_urange_k - coord_lrange_k + 1)) + coord_lrange_k;

                    count += 1;

                }
            }

            if (count == 25){   
                cerr << "Error: Failed to generate valid input for 25 simultaneous attempts." << endl;
                exit(0);
            }

            tuple <int, int> coord_3;
            coord_3 = make_tuple(x2, y2);
            verified_streets[last_name].push_back(coord_3);

            // cerr << "UPDATED GRAPH. IP COUNT = " << ip_count << endl;
            // print_graph(verified_streets);

        }

        count = 0;
        ip_count = 0;


        for (auto const& l : verified_streets){
                
            string name = l.first;  

            cout << "a \"" << name << "\" ";   
            unsigned int r;       
            vector < tuple<int,int> > meow = l.second; //Returns vector of tuples for each street
            for (r = 0; r < meow.size()-1; r++){
                int g = get<0>(meow[r]);
                int h = get<1>(meow[r]);

                cout << "(" << g << "," << h << ") ";

            }

            int g = get<0>(meow[r]);
            int h = get<1>(meow[r]);
            cout << "(" << g << "," << h << ") ";

            std::cout << std::endl;
        }

        cout << "g" << endl;

        urandom.read((char*)&num, sizeof(num));
        int wait_val = num%(wait_k - 5 + 1) + 5;

        
        sleep(wait_val);

    
    }


    urandom.close(); 


    return 0;
}
