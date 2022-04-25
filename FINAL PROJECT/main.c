#include "Source.h"

/*  NOTICE!!!!!!
*	Arrays stop at NULL as final index!!!
*/

void main() {
	int id;
	unsigned int instrumentAmount = 5;
	unsigned int musiciansAmount = 3;
	char** InstrumentFile = (char**)malloc((instrumentAmount + 1) * sizeof(char*));
	char** MusiciansFile = (char**)malloc((musiciansAmount + 1) * sizeof(char*));
	MusiciansFile[3] = NULL;
	char str[SIZE];
	InstrumentTree tree;
	Musician** archive;

	for (int i = 0; i < 3; i++) {
		MusiciansFile[i] = (char*)malloc(SIZE * sizeof(char));
		gets(MusiciansFile[i]);
		printf("Entered line: %s to line: %d\n", MusiciansFile[i], i);
	}

	for (int i = 0; i < 6; i++) {
		InstrumentFile[i] = (char*)malloc(10 * sizeof(char));
		if (i == 5) InstrumentFile[5] = NULL;
		else scanf("%s", InstrumentFile[i]);
	}
	

	tree = buildTree(InstrumentFile);
	
	printTree(tree.root);

	id = findInsId(tree, "Flute");
	printf("%d", id);


	archive = buildMusicianArchive(MusiciansFile, tree, 3);

	MCS* MCSizes = makeMCSArr(instrumentAmount);
	//printMusicianArchive(archive, 3);


}