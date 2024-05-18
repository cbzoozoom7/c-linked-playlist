#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PlaylistNode.h"
void PrintMenu(char playlistTitle[]) {
   printf("%s PLAYLIST MENU\n", playlistTitle);
   printf("a — Add song\n");
   printf("r — Remove song\n");
   printf("c — Change position of song\n");
   printf("s — Output songs by specific artist\n");
   printf("t — Output total time of playlist (in seconds)\n");
   printf("o — Output full playlist\n");
   printf("q — Quit\n");
}
const char *getString() {
   char *str = (char *) malloc(50 * sizeof(char));
   fgets(str, 50, stdin);
   if (str[strlen(str) - 1] != '\n') {
      while (getc(stdin) != '\n') {
         //intentionally empty
      }
   } else {
      str[strlen(str) - 1] = '\0';
   }
   return str;
}
int getInt() {
   int out = -1;
   char buffer[50];
   strcpy(buffer, getString);
   sscanf(buffer, "%d", &out);
   return out;
}
PlaylistNode* ExecuteMenu(char option, char playlistTitle[], PlaylistNode* headNode) {
   PlaylistNode *prev = NULL;
   PlaylistNode *current = headNode;
   char uid[50];
   char artist[50];
   int i;
   switch(option) {
      case 'q':
         while (current != NULL) {
            prev = current;
            current = GetNextPlaylistNode(current);
            free(prev);
         }
      break;
      case 'o':
         printf("%s — OUTPUT FULL PLAYLIST\n", playlistTitle);
         if (current == NULL) {
            printf("Playlist is empty\n");
         } else {
            int i = 1;
            while (current != NULL) {
               printf("%d.\n", i++);
               PrintPlaylistNode(current);
               printf("\n");
               current = GetNextPlaylistNode(current);
            }
         }
      break;
      case 'a':
         printf("ADD SONG\n");
         printf("Enter song's unique ID:\n");
         strcpy(uid, getString());
         printf("Enter song's name:\n");
         char name[50];
         strcpy(name, getString());
         printf("Enter artist's name:\n");
         strcpy(artist, getString());
	 int length = -1;
	 while (length <= 0) {
            printf("Enter song's length (in seconds):\n");
	    length = getInt();
	    if (length <= 0) {
	       printf("Invalid length. Length must be a positive integer.\n");
	    }
	 }
         PlaylistNode *newNode = (PlaylistNode *) malloc(sizeof(PlaylistNode));
         CreatePlaylistNode(newNode, uid, name, artist, length, NULL);
         if (current == NULL) {
            headNode = newNode;
         } else {
            while (GetNextPlaylistNode(current)) { //find the last node
               current = GetNextPlaylistNode(current);
            }
            SetNextPlaylistNode(current, newNode);
         }
      break;
      case 'r':
         printf("REMOVE SONG\n");
         printf("Enter song's unique ID:\n");
         strcpy(uid, getString());
         char found = 0;
         PlaylistNode *prev = NULL;
         while (current && !found) {
            if (strcmp(current->uniqueID, uid) == 0) {
               found = 1;
            } else {
               prev = current;
               current = GetNextPlaylistNode(current);
            }
         }
         if (current) {
            if (prev) {
               SetNextPlaylistNode(prev, GetNextPlaylistNode(current));
            } else {
               headNode = GetNextPlaylistNode(current);
            }
            char name[50];
            strcpy(name, current->songName);
            free(current);
            printf("\"%s\" removed.\n", name); //"All for You" removed.
         } else {
            printf("%s not found.\n", uid);
         }
      break;
      case 'c':
         printf("CHANGE POSITION OF SONG\n");
         printf("Enter song's current position:\n");
         int oldPos = getInt();
         printf("Enter new position for song:\n");
         int newPos = getInt();
         PlaylistNode *twoBack = NULL; //for use if newPos > oldPos
         prev = NULL;
         PlaylistNode *oldPrev = NULL;
         PlaylistNode *target = NULL;
         PlaylistNode *newPrev = NULL;
         i = 1;
         while (current && !target) {
            printf("i: %d\n", i);
            if (i == oldPos) {
               oldPrev = prev;
               target = current;
            }
            if (i == newPos) {
               newPrev = prev;
            }
            twoBack = prev;
            prev = current;
            current = GetNextPlaylistNode(current);
            i++;
         }
         printf("target: %p \tnewPrev: %p\n", target, newPrev);
         while (current && !newPrev) {
            printf("i: %d\n", i);
            if (i == newPos) {
               newPrev = twoBack;
            }
            twoBack = prev;
            prev = current;
            current = GetNextPlaylistNode(current);
            i++;
         }
         printf("target: %p \tnewPrev: %p\n", target, newPrev);
         if (newPos >= i) { //if newPos is past the end of the list, move target to the end of the list.
            newPrev = prev;
         }
         if (target && (newPrev || newPos <= 1)) {
            if (oldPrev) {
               printf("151\n");
               SetNextPlaylistNode(oldPrev, GetNextPlaylistNode(target));
            } else {
               printf("154\n");
               headNode = GetNextPlaylistNode(target);
            }
            printf("157\n");
            if (newPrev) {
               printf("158\n");
               InsertPlaylistNodeAfter(newPrev, target);
            } else {
               printf("161\n");
               headNode = target;
            }
            printf("\"%s\" moved to position %d\n", target->songName, newPos);
         } else {
            if (!target) {
               printf("Playlist has no position %d.\n", oldPos);
            }
            if (!newPrev) {
               printf("Failed to find new position.\n");
            }
         }
      break;
      case 's':
         printf("OUTPUT SONGS BY SPECIFIC ARTIST\n");
         printf("Enter artist's name:\n");
         strcpy(artist, getString());
         i = 1;
         int results = 0;
         while (current) {
            if (strcmp(current->artistName, artist) == 0) {
               printf("\n%d.\n", i);
               PrintPlaylistNode(current);
               results++;
            }
            current = GetNextPlaylistNode(current);
            i++;
         }
         if (results == 0) {
            printf("No results.\n");
         }
      break;
      case 't':
         printf("OUTPUT TOTAL TIME OF PLAYLIST (IN SECONDS)\n");
         int sum = 0;
         while (current) {
            sum += current->songLength;
            current = GetNextPlaylistNode(current);
         }
         printf("Total time: %d seconds\n", sum);
      break;
      default:
         printf("Option %c not implimented.\n", option);
   }
   return headNode;
}

int main(void) {
   printf("Enter playlist's title:\n");
   char playlistTitle[50];
   strcpy(playlistTitle, getString());
   char choice = '\0';
   PlaylistNode *head = NULL;
   while (choice != 'q') {
      choice = '\0';
      while (choice == '\0') {
         PrintMenu(playlistTitle);
         printf("Enter menu choice:\n");
         choice = getString()[0];
         //scanf("%c", &choice);
         switch(choice) {
            case 'a':
            case 'r':
            case 'c':
            case 's':
            case 't':
            case 'o':
            case 'q':
               break;
            default:
               printf("Invalid. Try again or q to quit.\n");
               choice = '\0';
         }
      }
      head = ExecuteMenu(choice, playlistTitle, head);
   }
   return 0;
}
/*test input
1 3 5 7 9 11 14 17 20 23 26 29 32 35 38 41 44 47.
s
null artist
t
o
a
id1
name1
artist1
100
s
null artist
s
artist1
t
o
a
id2
name2
artist2
240
s
artist1
s
artist2
a
id3
name3
artist3
30
t
a
id4
name4
artist4
420
o
a
id5
name5
artist5
58
o
c
1
5
o
c
5
1
o
c
2
3
o
c
7
10
o
c
4
99
o
q
*/
