//PlaylistNode.h
//Christopher Bowman
//Created Tue 14 Nov 2023
typedef struct STRUCT_PLAYLIST_NODE {
   char uniqueID[50];
   char songName[50];
   char artistName[50];
   int songLength;
   struct STRUCT_PLAYLIST_NODE *nextNodePtr;
} PlaylistNode;
void CreatePlaylistNode(PlaylistNode *, char[], char[], char[], int, PlaylistNode *);
void InsertPlaylistNodeAfter(PlaylistNode *, PlaylistNode *);
void SetNextPlaylistNode(PlaylistNode *, PlaylistNode *);
PlaylistNode* GetNextPlaylistNode(PlaylistNode *);
void PrintPlaylistNode(PlaylistNode *);
