
#include "Source.h"

//******************************************QUESION1*************************

InstrumentTree buildTree(char** InstrumentFile) {
	InstrumentTree tree = makeEmptyTree();
	IDX index = 0;
	TreeNode* tempPtr;
	char* tempInstrument = (char*)malloc(SIZE * sizeof(char));
	checkMemoryAllocation(tempInstrument);
	while (InstrumentFile[index] != NULL) {
		strcpy(tempInstrument, InstrumentFile[index]); //to be replaced by fscanf
		tempPtr = buildInstrumentNode(tempInstrument, index);
		tree.root = addInstrumentToTree(tree.root, tempPtr);
		index++;
	}
	//printTree(tree.root);
	return tree;
}

void printTree(TreeNode* root) {
	if (root == NULL) return;
	else {
		printTree(root->left);
		printf("%s , ", root->instrument);
		printTree(root->right);
	}
}

InstrumentTree makeEmptyTree() {
	InstrumentTree tree;
	tree.root = NULL;
	return tree;
}
TreeNode* buildInstrumentNode(char* instrument, IDX index) {
		
	TreeNode* instNode = (TreeNode*)malloc(sizeof(TreeNode));
	checkMemoryAllocation(instNode);

	instNode->instrument = (char*)malloc((strlen(instrument) + 1) * sizeof(char));
	checkMemoryAllocation(instNode->instrument);

	strcpy(instNode->instrument, instrument);
	instNode->insId = (unsigned short)index;
	instNode->left = instNode->right = NULL;

	return instNode;
}
TreeNode* addInstrumentToTree(TreeNode* root, TreeNode* currInst) { //Helper
	if (root == NULL) 
		root = currInst;
	else {
		if (isRootFirst(root->instrument, currInst->instrument))
			root->left = addInstrumentToTree(root->left, currInst);
		else
			root->right = addInstrumentToTree(root->right, currInst);
	}
	return root;
}

bool isRootFirst(char* root, char* instName) {
	IDX ch = 0;

	while (root[ch] == instName[ch])
		ch++;

	if ((int)root[ch] < (int)instName[ch])
		return true;
	else return false;

}

//******************************************QUESION2*************************

int findInsId(InstrumentTree tree, char* instrument) {
	return findInsIdHelper(tree.root, instrument);
}

int findInsIdHelper(TreeNode* root, char* instrument) {
	int id;
	if (root == NULL)
		return -1;
	if (strcmp(root->instrument, instrument) == 0) 
		return root->insId;

	if (isRootFirst(root->instrument, instrument))
		id = findInsIdHelper(root->left, instrument);
	else 
		id = findInsIdHelper(root->right, instrument);
	return id;
}

//******************************************QUESION3*************************

//NOTE TO SELVES - MusiciansAmount will be received by counting the lines in the musiciansFile!

Musician** buildMusicianArchive(char** musiciansFile, InstrumentTree tree, int MusiciansAmount) { 
	IDX musician = 0;
	
	Musician** MusiciansGroup = (Musician**)malloc(MusiciansAmount * sizeof(Musician*));
	checkMemoryAllocation(MusiciansGroup);

	const char* seps = " ,.;:?!-\t'()[]{}<>~_";

	while (musiciansFile[musician] != NULL) {
		MusiciansGroup[musician] = addMusicianToArchive(musiciansFile[musician], tree, seps);
		musician++;
	}

	return MusiciansGroup;
}

Musician* addMusicianToArchive(char* musician, InstrumentTree tree, char* seps) {
	Musician* musicianPtr = makeEmptyMusician();
	bool isName = true;
	unsigned int ID;
	char* token = strtok(musician, seps);
	printf("NAME: %s\n", token); //DEBUG
	IDX i = 0;
	unsigned int logSize = 0;
	unsigned int physSize = MIN_MUSICIAN_NAME_NUM;
	MPINode* curr = musicianPtr->instruments.first;

	musicianPtr->name = (char**)malloc(physSize * sizeof(char*));
	checkMemoryAllocation(musicianPtr->name);

	while (isName) {
		ID = findInsId(tree, token);
		if (ID != INST_NOT_FOUND) isName = false;
		else {
			if (logSize == physSize) {
				physSize *= 2;
				musicianPtr->name = (char**)realloc(musicianPtr->name, physSize * sizeof(char*));
				checkMemoryAllocation(musicianPtr->name);
			}
			
			//check if strlen(token) works
			musicianPtr->name[i] = (char*)malloc(strlen(token) * sizeof(char));
			checkMemoryAllocation(musicianPtr->name[i]);

			strcpy(musicianPtr->name[i], token);

			token = strtok(NULL, seps);

			logSize++;
			i++;
		}
	}

	if (physSize != logSize)
		musicianPtr->name = (char**)realloc(musicianPtr->name, logSize * sizeof(char*));
	checkMemoryAllocation(musicianPtr->name);

	musicianPtr->namesAmount = logSize;

	while (token != NULL) {
		MPINode* node = makeEmptyNode();
		node->MPI.insId = ID;

		token = strtok(NULL, seps);
		node->MPI.price = atof(token);

		insertNodeToEndList(&(musicianPtr->instruments), node);

		token = strtok(NULL, seps); //CHECKED
		if (token != NULL) ID = findInsId(tree, token);
		//printf("Instrument ID: %d, Price: %f\n", musicianPtr->instruments.last->MPI.insId, musicianPtr->instruments.last->MPI.price); //DEBUG
	}

	return musicianPtr;
}

Musician* makeEmptyMusician() {
	Musician* musicianPtr = (Musician*)malloc(sizeof(Musician));
	checkMemoryAllocation(musicianPtr);

	musicianPtr->name = NULL;
	musicianPtr->instruments = makeEmptyMPIList();

	return musicianPtr;
}

MPIList makeEmptyMPIList() {
	MPIList lst;
	lst.first = NULL;
	return lst;
}

MPINode* makeEmptyNode() {
	MPINode* nodePtr = (MPINode*)malloc(sizeof(MPINode));
	checkMemoryAllocation(nodePtr);

	nodePtr->next = NULL;
	return nodePtr;
}

void insertNodeToEndList(MPIList* lst, MPINode* node) { //CHECKED
	if (lst->first == NULL)
		lst->first = lst->last = node;
	else {
		lst->last->next = node;
		lst->last = node;
	}
}

void printMusicianArchive(Musician** archive, int musiciansAmount) {
	IDX musician, name;
	MPINode* curr;
	printf("\n\n");
	for (musician = 0; musician < musiciansAmount; musician++) {
		printf("Musician number #%d\n", musician + 1);

		printf("Full name: ");

		for (name = 0; name < archive[musician]->namesAmount; name++) {
			//printf("Name #%d: ", name);
			printf("%s ", archive[musician]->name[name]);
		}
		printf("\n");

		printf("Instruments & price:\n");

		curr = archive[musician]->instruments.first;

		while (curr != NULL) {
			printf("Instrument ID: %d\tPrice:%f\n", curr->MPI.insId, curr->MPI.price);
			curr = curr->next;
		}

	}

}

void printMusiciansCollection(Musician*** MusCollection, MCS* MCSizes, unsigned int instrumentAmount) {
	IDX instrument, musician;
	for(instrument = 0; instrument < instrumentAmount; instrument++) {
		printf("~~~Musicians who play on instrument #%d:~~~\n", instrument);

		for(musician = 0; musician < MCSizes[instrument].logSize; musician++){
			printf("%s", MusCollection[instrument][musician]->name[0]);
			if (musician == MCSizes[instrument].logSize - 1) printf(", ");
			else printf("\n");
		}
	}
}

//******************************************QUESTION4*************************

//NOTE TO SELVES: We pretend to have the node amount- need to get it from somewhere!
//We pretend that we have the node amount for now.

Musician*** buildMusiciansCollection(int instrumentAmount, Musician** MusiciansGroup, int MusiciansAmount, MCS* MCSizes) {

	Musician*** MusiciansCollection = (Musician***)malloc(instrumentAmount * sizeof(Musician**));
	checkMemoryAllocation(MusiciansCollection);

	MusiciansCollection = fillMusiciansCollection(MusiciansGroup, MusiciansCollection, MCSizes, MusiciansAmount, instrumentAmount);

	return MusiciansCollection;
}

Musician*** fillMusiciansCollection(Musician** MusiciansGroup, Musician*** MusiciansCollection, MCS* MCSizes, int MusiciansAmount, int instrumentAmount) {
	IDX index;

	for (index = 0; index < MusiciansAmount; index++) {
		MusiciansCollection = addMusicianToCollection(MusiciansGroup[index], MusiciansCollection, MCSizes);
	}

	for (index = 0; index < instrumentAmount; index++) {
		if ((MCSizes[index].logSize != MCSizes[index].physSize) && (MCSizes[index].physSize != 0)) {
			MusiciansCollection[index] = (Musician**)realloc(MusiciansCollection[index], MCSizes[index].logSize);
			checkMemoryAllocation(MusiciansCollection[index]);
		}
		else if (MCSizes[index].physSize == 0)
			MusiciansCollection[index] = NULL;
	}
	return MusiciansCollection;
}

Musician*** addMusicianToCollection(Musician* musician, Musician*** MusiciansCollection, MCS* MCSizes) {
	unsigned short insId;
	MPINode* curr = musician->instruments.first;
	while (curr != NULL) {
		insId = curr->MPI.insId;

		if (MCSizes[insId].physSize == 0) {
			MCSizes->physSize = 1;
			MusiciansCollection[insId] = (Musician**)malloc(sizeof(Musician*));
			checkMemoryAllocation(MusiciansCollection[insId]);
		}

		if (MCSizes[insId].logSize == MCSizes[insId].physSize) {
			MCSizes->physSize *= 2;
			MusiciansCollection[insId] = (Musician**)realloc(MusiciansCollection[insId], MCSizes->physSize * sizeof(Musician*));
			checkMemoryAllocation(MusiciansCollection[insId]);
		}

		MusiciansCollection[insId][MCSizes->logSize] = musician;

		MCSizes->logSize++;
		curr = curr->next;
	}
	return MusiciansCollection;
}

MCS* makeMCSArr(int instrumentAmount) {
	IDX index;
	MCS* MCSizes = (MCS*)malloc(instrumentAmount * sizeof(MCS));
	checkMemoryAllocation(MCSizes);

	for (index = 0; index < instrumentAmount; index++)
		MCSizes[index].logSize = MCSizes[index].physSize = 0;

	return MCSizes;
}

void printCollection(Musician*** MusiciansCollection, MCS* MCSizes);

//******************************************MEMORY & FILES*************************

void checkMemoryAllocation(void* ptr) {
	if (ptr == NULL) {
		printf("FATAL ERROR! MEMORY ALLOCATION FAILED!\n");
		exit(1);
	}
}

void checkFile(FILE* filePtr) {
	if (filePtr == NULL) {
		printf("Failed opening the file. Exiting!\n");
		exit(1);
	}
}