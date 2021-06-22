#include "../include/builder.hpp"
#include <algorithm>

using namespace std;

/*
 * A builder for the HHCP instances and all related functions that are required!
 */

namespace Builder{
	namespace HHCP{
		void demand_random_nodes(const Configurations& con, std::vector<Node>& nodes);

		int build(const Configurations& con, const std::vector<GeoLocation>& locations, std::vector< std::vector< double > >& matrix, Instance& inst){
			if(con.verbose >= 2) printf("Grouping nodes...\n");
			vector<Node> nodes;
			for(size_t id = 0; id < locations.size(); id++){
				nodes.push_back(Node(static_cast<int>(id), locations[id]));
			}
			nodes[0].type = NodeType::Depot; // node 0 is always the depot

			if(con.verbose >= 2){
				printf("DONE: Nodes grouped.\n");
				printf("Setting demands...\n");
			}

			demand_random_nodes(con, nodes);
			if(con.verbose >= 2){
				printf("DONE: Set demands.\n");
			}

			inst.nodes = nodes;
			inst.matrix = matrix;

         std::vector <GeoLocation> locs;
         for (auto &n: nodes)
            locs.push_back(n.loc);
         inst.hhcp_data->setLocations(locs);
         for (size_t i = 0; i < locs.size(); ++i) {
            for (size_t j = 0; j < locs.size(); ++j) {
               inst.hhcp_data->setDistance(i, j, matrix[i][j]);
            }
         }

			return EXIT_SUCCESS;
		}


		void demand_random_nodes(const Configurations& con, std::vector<Node>& nodes){
			//TODO: need to check this hard-coded demand and make it a configuration
			std::uniform_int_distribution<int> rdemand(1,5);
			nodes[0].demand = 0;
			for(size_t id=1;id<nodes.size();id++){
				nodes[id].demand = rdemand(rng);
			}
		}
	};
};
