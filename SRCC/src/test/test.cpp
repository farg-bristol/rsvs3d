#ifdef DBG_MEMLEAK
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>
#endif //DBG_MEMLEAK

#include <iostream>
#include <cstdlib>
#include <string>
#include <functional>

#include "test.hpp"
#include "arraystructures.hpp"

#ifdef TEST_ALL
#include "postprocessing.hpp"
#include "voxel.hpp"
#endif //TEST_ALL
#include "mesh.hpp"
#include "snake.hpp"
#include "snakeengine.hpp"
#include "meshrefinement.hpp"
#include "snakevel.hpp"
#include "RSVSalgorithm.hpp"

int main(){
	customtest gridTest;



	#ifdef TEST_ALL
	gridTest.Run(Test_ArrayStructures,"arraystructures");
	
	gridTest.Run(Test_BuildBlockGrid_noout,"Voxel");

	gridTest.Run(Test_tecplotfile,"post-processing");

	gridTest.Run(Test_tecplotfile,"post-processing class");

	gridTest.Run(Test_snakeOrderEdges,"Snake Order error");
	gridTest.Run(Test_SnakeStructures,"Snake containers");
	gridTest.Run(TestTemplate_ArrayStruct<vert>,"TestTemplate_ArrayStruct<vert>");
	gridTest.Run(TestTemplate_ArrayStruct<edge>,"TestTemplate_ArrayStruct<edge>");
	gridTest.Run(TestTemplate_ArrayStruct<surf>,"TestTemplate_ArrayStruct<surf>");
	gridTest.Run(TestTemplate_ArrayStruct<volu>,"TestTemplate_ArrayStruct<volu>");
	gridTest.Run(TestTemplate_ArrayStruct<snax>,"TestTemplate_ArrayStruct<snax>");
	gridTest.Run(TestTemplate_ArrayStruct<snaxedge>,"TestTemplate_ArrayStruct<snaxedge>");
	gridTest.Run(TestTemplate_ArrayStruct<snaxsurf>,"TestTemplate_ArrayStruct<snaxsurf>");
	gridTest.Run(Test_snakeinitflat,"Snake spawning 2D");
	gridTest.Run(Test_surfcentre,"test SurfCentroid"); 
	gridTest.Run(Test_MeshRefinement,"Multi-Level Meshes");
	gridTest.Run(Test_RSVSalgo_init,"RSVS spawn");
	gridTest.Run(Test_snakeinit,"Snake rand velocity"); // Non working tests
	gridTest.Run(Test_MeshOut,"Mesh output"); 
	#endif //TEST_ALL
	

	// gridTest.Run(Test_RSVSalgo,"Snake RSVS from spawn");
	// gridTest.Run(Test_MeshOrient,"Output mesh orientation");
	// gridTest.Run(Test_snakeRSVS_singlevol,"Snake RSVS single vol");
	gridTest.Run(Test_RSVSalgo_singlevol,"Snake RSVS single vol");
	// gridTest.Run(Test_snakeRSVS,"Snake RSVS");




	gridTest.PrintSummary();



	return(0);
}

