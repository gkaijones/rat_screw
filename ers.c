/*
 *  Egyptian Ratscrew
 *      A two-player implementation of the card game Egyptian Ratscrew.
 *      Meant as an exploratory programming exercise.
 *      rev 1
 *
 *  Author: Griffin Jones
 *          gkaijones@gmail.com
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define SIZE 52 // size of deck


struct stack {
    int size;
    int cards[SIZE];
};

enum suit {
    HEARTS = 0,
    CLUBS = 1,
    DIAMONDS = 2,
    SPADES = 3
};

enum face {
    TWO = 0,
    THREE = 1,
    FOUR = 2,
    FIVE = 3,
    SIX = 4,
    SEVEN = 5,
    EIGHT = 6,
    NINE = 7,
    TEN = 8,
    JACK = 9,
    QUEEN = 10,
    KING = 11,
    ACE = 12
};

/*---------------->FUNCTIONS<---------------*/

/* PRINT STACK -- 
 * A utility function used to expose the contents of a stack structure. */
void printStack(struct stack *deck)
{
    printf("Stack has %d card(s) in it.\n", deck->size);
    for( int i = 0; i < deck->size; i++)
    {
        printf("%d\n", deck->cards[i]);
    }
}

/* KNUTH SHUFFLE --
 * Used to randomize the contents of a stack */
void shuffle(struct stack *deck)
{
    int i = deck->size;   // iterator
    int j;  // random selection
    int tmp;

    while(--i > 0)
    {
        j = rand() % i; 
        tmp = deck->cards[i];
        deck->cards[i] = deck->cards[j]; 
        deck->cards[j] = tmp;
    }
}

/* GET SUIT --
 * returns an int representing the suit of a card*/
int getSuit(int card){
    int suit = card / 13;
    return suit;
}

/* GET FACE --
 * returns an int representing the face-value of a card. */
int getFace(int card){
    int face = card % 13;
    return face;
}

/* PRINT CARD --
 * prints out the face and suit in ENGLISH. */
void printCard(int face, int suit){
    char f[10]; // face string
    char s[10]; // suit string
    switch(face){
        case TWO:
            strcpy(f, "Two");
            break;
        case THREE:
            strcpy(f, "Three");
            break;
        case FOUR: 
            strcpy(f, "Four");
            break;
        case FIVE:
            strcpy(f, "Five");
            break;
        case SIX:
            strcpy(f, "Six");
            break;
        case SEVEN: 
            strcpy(f, "Seven");
            break;
        case EIGHT:
            strcpy(f, "Eight");
            break;
        case NINE: 
            strcpy(f, "Nine");
            break;
        case TEN: 
            strcpy(f, "Ten");
            break;
        case JACK:
            strcpy(f, "Jack");
            break;
        case QUEEN:
            strcpy(f, "Queen");
            break;
        case KING:
            strcpy(f, "King");
            break;
        case ACE:
            strcpy(f, "Ace");
            break;
    }
    switch(suit){
        case HEARTS:
            strcpy(s, "Hearts");
            break;
        case CLUBS:
            strcpy(s, "Clubs");
            break;
        case DIAMONDS:
            strcpy(s, "Diamonds");
            break;
        case SPADES:
            strcpy(s, "Spades");
            break;
    }
    printf("%s of %s\n", f, s); 
}

/* NEXT CARD --
 * returns an integer representing the number of cards that must be played in response to a given card */
int nextCard(int face){
    if(face < 9){
        return 0;           // No Challenge
    } 
    else if (face == JACK){
        return 1;           // 1 Challenge Card
    }
    else if(face == QUEEN){
        return 2;           // 2 Challenge Cards
    }
    else if(face == KING){
        return 3;           // 3 Challenge Cards
    }
    else{
        return 4;           // 4 Challenge Cards
    } 
}

/* PLAY CARD --
 * Move top card from one stack to top of another. 
 * Adjust the stack sizes.*/
void playCard(struct stack *deck, struct stack *table){
    deck->size--;   //Decrement first to correct for off-by-one
    table->cards[table->size] = deck->cards[deck->size];
    table->size++;
}

/* MOVE CARDS --
 * Move all cards from one stack to the end of the other stack. */
void moveCards(struct stack *from, struct stack *to){
    if(from->size == 0){ 
        ;   // If there are no cards in the from stack, do nothing.
    } else {
        memmove(&to->cards[from->size],
                &to->cards[0],
                from->size * sizeof(from->cards[0]));   // Offset cards in to-stack by the number of cards in the from-stack.

        memcpy(&to->cards[0],
                &from->cards[0],
                from->size * sizeof(from->cards[0]));   // Copy all the cards in the from-stack to the offset area in the to-stack.

        to->size += from->size; 
        from->size = 0;     // Update the stack sizes accordingly.
    }
}

/* ASSIGN CURRENT PLAYER --
 * Returns a pointer to a stack structure, determined by a boolean variable cp.
 * The pointer points to one of the two player stacks.*/
struct stack *assignCurrentPlayer(bool cp, struct stack *player1, struct stack *player2){
    struct stack *currentPlayer;
    if(cp){ // Who is Current Player?
        currentPlayer = player1;
    }
    else{
        currentPlayer = player2;
    }
    return(currentPlayer);
}
/*--------------------->DECLARATIONS<----------------*/
struct stack deck; 
struct stack table;
struct stack player1;
struct stack player2;
struct stack *currentPlayer;
bool cp = true; // TRUE is player1, FALSE is player2
bool challenge = false; // flag to indicate if in challenge mode.
bool challenge_complete = false;
int topCard;
int num_challenges;
int rounds = 1;
/*--------------------->MAIN LOOP<-------------------*/
int main(){
    for( int i = 0; i < SIZE; i++){
        deck.cards[i] = i;  // Make a deck.
    }

    shuffle(&deck); // Shuffle the deck of cards numbered 0-51.

    player1.size = SIZE / 2;
    for(int i = 0; i < player1.size; i++){
        player1.cards[i] = deck.cards[i]; // Assign the first half of the shuffled deck to player 1.
    }

    player2.size = SIZE / 2;
    for(int i = 0; i < player2.size; i++){
        player2.cards[i] = deck.cards[i+SIZE/2]; // Assign the second half of the shuffled deck to Player 2.
    }
    
/*--------------------->GAME LOOP<-------------------*/
    while(rounds < 150){
        printf("ROUND %d:\n", rounds); // Print Round Number at start of loop.
start:
        currentPlayer = assignCurrentPlayer(cp, &player1, &player2); // Reassign current player.
        
        if (challenge_complete){ // If the challenge is complete, move the cards on the table to the current player's hand.
            moveCards(&table, currentPlayer);  
            challenge_complete = false;     // Reset Flag
        } 

        if (challenge){ 
            num_challenges = nextCard(table.cards[table.size]); // get the number of chances that the current player has.
            while (num_challenges > 0){
                playCard(currentPlayer, &table);
                topCard = getFace(table.cards[table.size]);
                if ((topCard == JACK) | (topCard == QUEEN) | (topCard == KING) | (topCard == ACE)){
                    cp = !cp; // swap current players
                    goto start; // re-initiate challege with new current player.
                }
            }
            challenge_complete = true; // After all challenges exhausted, set victory flag.
        }
        else { // Default Play Mode. Player who wins a challenge starts.
            playCard(currentPlayer, &table); // Current player plays a card on the table
        
            topCard = getFace(table.cards[table.size]);
            if ((topCard == JACK) | (topCard == QUEEN) | (topCard == KING) | (topCard == ACE)){
                challenge = true;
            }
        }

        cp = !cp; // get ready to swap players.
        rounds++; // Next Round.

    }
    printf("CARDS ON TABLE:\n");
    printStack(&table);
    printf("PLAYER 1: \n");
    printStack(&player1);
    printf("PLAYER 2: \n");
    printStack(&player2);
}
