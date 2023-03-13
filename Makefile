# CSE 374 AU 22
# Noah Crouch
# 11/28/22


# Make file for CSE 374 HW5

# Make t9 again if tnine.o or trie.o change
t9: tnine.o trie.o
	gcc -Wall -g -std=c11 -o t9 tnine.o trie.o

# Make trie.o again if trie.c or trieNode.h changes
trie.o: trie.c trienode.h
	gcc -Wall -g -std=c11 -c trie.c

# Make tnine.o again  if tnine.c or trieNode.h changes
tnine.o: tnine.c trienode.h
	gcc -Wall -g -std=c11 -c tnine.c

# Rm any files ending with .o and t9 output file when clean is called
clean:
	rm -f *.o t9
