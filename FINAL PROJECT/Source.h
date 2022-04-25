#ifndef defbool
#define defbool
#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#endif

#define SIZE 150
#define INST_NOT_FOUND -1
#define MIN_MUSICIAN_NAME_NUM 2

typedef unsigned int IDX;

typedef struct treeNode {
	char* instrument; //Instrument name
	unsigned short insId; //Instrument ID
	struct treeNode* left;
	struct treeNode* right;
} TreeNode;

typedef struct tree {
	TreeNode* root;
} InstrumentTree;

typedef struct {
	unsigned short insId;
	float price;
} MusicianPriceInstrument;

typedef struct mpinode {
	MusicianPriceInstrument MPI;
	struct mpinode* next;
}MPINode;

typedef struct mpilist {
	MPINode* first;
	MPINode* last;
}MPIList;

typedef struct {
	char** name;
	unsigned int namesAmount;
	MPIList instruments;
} Musician;

typedef struct {
	unsigned int logSize;
	unsigned int physSize;
}MCS; //MusicianCollectionSizes

//1st question
InstrumentTree buildTree(char** InstrumentFile);
InstrumentTree makeEmptyTree();
TreeNode* buildInstrumentNode(char* instrument, IDX index);
TreeNode* addInstrumentToTree(TreeNode* root, TreeNode* Instrument); //Helper
bool isRootFirst(char* root, char* instName);

void printTree(TreeNode* root);

//2nd question
int findInsId(InstrumentTree tree, char* instrument);
int findInsIdHelper(TreeNode* root, char* instrument);

//3rd question
Musician** buildMusicianArchive(char** MusiciansFile, InstrumentTree tree, int MusiciansAmount);
Musician* addMusicianToArchive(char* musician, InstrumentTree tree, char* seps);
Musician* makeEmptyMusician();
MPIList makeEmptyMPIList();
MPINode* makeEmptyNode();
void insertNodeToEndList(MPIList* lst, MPINode* node);
//void printMusicianArchive(Musician** archive, int musiciansAmount); //DEBUG

//4th Question
Musician*** buildMusiciansCollection(int instrumentAmount, Musician** MusiciansGroup, int MusiciansAmount, MCS* MCSizes);
Musician*** fillMusiciansCollection(Musician** MusiciansGroup, Musician*** MusiciansCollection, MCS* MCSizes, int MusiciansAmount, int instrumentAmount);
Musician*** addMusicianToCollection(Musician* musician, Musician*** MusiciansCollection, MCS* MCSizes);
MCS* makeMCSArr(int instrumentAmount);

void printMusiciansCollection(Musician*** MusCollection, MCS* MCSizes, unsigned int instrumentAmount); //DEBUG

//Memory
void checkMemoryAllocation(void* ptr);
void checkFile(FILE* filePtr);