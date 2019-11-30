#define SUITS 4
#define FACES 13
#define CARDS 52
#define KCARDS 5

#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

void shuffling_cards(int[][FACES]);
void dealingCards(const int[][FACES], const int, int[][KCARDS]);
void printCards(const int[][KCARDS], string[], string[], int);
void sort(int[], int);
bool fourOfAKind(const int[][KCARDS], int);
bool fullHouse(const int[][KCARDS], int);
int flush(const int[][KCARDS], int);
int straight(const int[][KCARDS], int);
int threeOfAKind(const int[][KCARDS], int);
int twoPairs(const int[][KCARDS], int);
int isPair(const int[][KCARDS], int);
int getHighestCard(const int[][KCARDS], int);
int getStatusOfHand(const int[][KCARDS], int);
void playersRank(int[], int [][KCARDS], int[], const int);
int playerScore(int[], const int [], int, const int);

int main()
{
	int deck[SUITS][FACES] = { 0 };
	string suits[SUITS] = { "Hearts", "Diamonds", "Clubs", "Spades" };
	string faces[FACES] = { "Ace", "Deuce", "Three", "Four", "Five"
	, "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King" };

	/*int numbers; 
	cout << "How many players will play this game? ";
	cin >> numbers;*/
	const int players = 5;;// tao mang 2 chieu chua bai cua n nguoi choi
	int hand[players][KCARDS];

	int scores[players] = { 0 };
	int totalScores[players] = { 0 };

	shuffling_cards(deck); // xao bai
	dealingCards(deck, players, hand);    //chia bai cho n nguoi choi
	printCards(hand, suits, faces, players); // ham doc la bai cua n nguoi choi
	int rank[players] = { 0 };
	playersRank(scores, hand, rank, players);	// ham xep hang nguoi choi moi vang
	for (int i = 0; i < players; i++)
		totalScores[i] += playerScore(rank, scores, i, players);  //ham cong diem cho nguoi choi
	for (int val : totalScores)
		cout << val << " ";

	return 0;
}

//Ham xao bai ngau nhien
void shuffling_cards(int deck[SUITS][FACES])
{
	unsigned seed = time(0);
	srand(seed);

	for (int i = 1; i <= CARDS; i++)
	{
		int suit = rand() % 4; // lay ngau nhien chat
		int face = rand() % 13; // lay ngau nhien gia tri
		while (deck[suit][face] != 0) // kiem tra phan tu thu [suit][face] da ton tai chua
		{
			suit = rand() % 4;
			face = rand() % 13;
		}

		deck[suit][face] = i;
	}
}

//ham doc la bai cua nguoi choi
void printCards(const int hand[][KCARDS], string suits[], string faces[], int players)
{
	for (int player = 0; player < players; player++)
	{
		cout << "Cards of player " << player + 1 << endl;
		for (int i = 0; i < KCARDS; i++)
		{
			int suit = hand[player][i] / 13;
			int face = hand[player][i] % 13;
			for (int k = 0; k < SUITS; k++)
				if (k == suit) cout << "(" << suits[k] << ",";
			for (int k = 0; k < FACES; k++)
				if (k == face) cout << faces[k] << ")";
			cout << endl;
		}
	}
}

//ham chia bai
void dealingCards(const int deck[SUITS][FACES], const int players, int hand[][KCARDS])
{

	for (int player = 0; player < players; player++)
	{
		int count = 0;
		for (int i = player + 1; i <= players*4 + player + 1; i += players)
		{
			for (int j = 0; j < SUITS; j++)
				for (int k = 0; k < FACES; k++)
					if (deck[j][k] == i)
					{
						hand[player][count] = j * FACES + k;
						count++;
					}
		}
	}

}

// ham sap xep 1 mang
void sort(int needSorting[], int length)
{
	for (int i = 0; i < length; i++)
	{
		int minIndex = i;
		int minValue = needSorting[i];

		for (int k = i; k < length; k++)
			if (needSorting[k] < minValue)
			{
				minValue = needSorting[k]; //neu phan tu thu k nho hon thi min = a[k], minIndex = k
				minIndex = k;
			}
		needSorting[minIndex] = needSorting[i];  // doi cho a[i] voi a[min]
		needSorting[i] = minValue;
	}
}

// ham kiem tra four of a kind
bool fourOfAKind(const int hand[][KCARDS], int player)
{
	int check[FACES] = { 0 }; // tao mang chua 13 gia tri 

	for (int i = 0; i < FACES; i++) 
	{
		for (int j = 0; j < KCARDS; j++)
			if (hand[player][j] % FACES == i) check[i]++;
	}

	for (int i = 0; i < FACES; i++) // kiem tra co gia tri nao bang 4 khong?
	{
		if (check[i] == 4) return true;
	}

	return false;
}

// ham kiem tra full house
bool fullHouse(const int hand[][KCARDS], int player)
{
	int trip, doub;
	int countTrip = 0, countDou = 0;

	trip = hand[player][0] % FACES; //gan gia tri dau tien cho gia tri thu 1
	for (int i = 1; i < KCARDS; i++) //tim gia tri cho cap 2
		if (trip != hand[player][i] % FACES)
		{
			doub = hand[player][i] % FACES;
			break;
		}

	for (int i = 0; i < KCARDS; i++) // kiem tra so lan  1 xuat hien
		if (hand[player][i] % FACES == trip) countTrip++;
	for (int i = 0; i < KCARDS; i++) // kiem tra so lan 2 xuat hien
		if (hand[player][i] % FACES == doub) countDou++;

	// 1 trong 2 se la cap 2 hoac 3
	if ((countTrip == 3 && countDou == 2) || (countTrip == 2 && countDou == 3)) return true;
	else return false;
}

// ham kiem tra flush
int flush(const int hand[][KCARDS], int player)
{
	int value = hand[player][0] / FACES; // gan chat can tim la phan tu dau tien
	int countValue = 0;

	for (int i = 0; i < KCARDS; i++)
		if ((hand[player][i] / FACES) == value) // kiem tra phan tu thu i co bang chat hay khong
			countValue++;
	if (countValue == 5) return 1;
	else return 0;
}

// ham kiem tra straight
int straight(const int hand[][KCARDS], int player)
{
	int check[KCARDS];

	for (int i = 0; i < KCARDS; i++)
		check[i] = hand[player][i] % FACES;

	sort(check, KCARDS); //sort de gia tri cua ham check tang dan;

	for (int i = 1; i < KCARDS; i++) //kiem tra gia tri thu co bang gia tri (thu - 1) - 1 khong
		if (check[i] != (check[i - 1] + 1)) return 0;

	return 1;
}

// ham kiem tra three of a kind
int threeOfAKind(const int hand[][KCARDS], int player)
{
	int check[FACES] = { 0 };

	for (int i = 0; i < FACES; i++) //tang phan tu gia tri
	{
		for (int j = 0; j < KCARDS; j++)
			if (hand[player][j] % FACES == i) check[i]++;
	}

	for (int i = 0; i < FACES; i++) // kiem tra co gia tri nao bang 3 khong?
	{
		if (check[i] == 3) return true;
	}

	return false;
}

//ham kiem tra two pairs
int twoPairs(const int hand[][KCARDS], int player)
{
	int	dou1, dou2;
	int countDou1 = 0, countDou2 = 0;

	dou1 = hand[player][0] % 13; //gan gia tri dau tien cho gia tri thu 1
	for (int i = 1; i < KCARDS; i++) //tim gia tri cho cap 2
		if (dou1 != hand[player][i] % FACES)
		{
			dou2 = hand[player][i] % FACES;
			break;
		}

	for (int i = 0; i < KCARDS; i++) // kiem tra so lan  1 xuat hien
		if (hand[player][i] % FACES == dou1) countDou1++;
	for (int i = 0; i < KCARDS; i++) // kiem tra so lan 2 xuat hien
		if (hand[player][i] % FACES == dou2) countDou2++;

	if (countDou1 == 2 && countDou2 == 2) return 1;
	else return 0;
}

//ham kiem tra pair
int isPair(const int hand[][KCARDS], int player)
{
	int countPair = 1;
	int check[KCARDS];
	
	for (int i = 0; i < KCARDS; i++)
		check[i] = hand[player][i] % FACES;
	sort(check, KCARDS);

	for (int i = 1; i < KCARDS; i++)
	{
		int front = check[i];
		int back = check[i - 1];
		if (front == back) countPair++;
	}

	if (countPair == 2) return 1;
	else return 0;
}

//ham kiem tra la bai lon nhat
int getHighestCard(const int hand[][KCARDS], int player)
{
	int maxCard = hand[player][0] % FACES;

	for (int i = 1; i < KCARDS; i++)
	{
		if (hand[player][i] % FACES > maxCard) maxCard = hand[player][i] % FACES;
	}

	return maxCard;
}

// ham tra ve trang thai cua 5 la bai
int getStatusOfHand(const int hand[][KCARDS], int player)
{
	int status;

	if (straight(hand, player) && flush(hand, player))
		status = 8;
	else if (fourOfAKind(hand, player))
		status = 7;
	else if (fullHouse(hand, player))
		status = 6;
	else if (flush(hand, player))
		status = 5;
	else if (straight(hand, player))
		status = 4;
	else if (threeOfAKind(hand, player))
		status = 3;
	else if (twoPairs(hand, player))
		status = 2;
	else if (isPair(hand, player))
		status = 1;
  //neu nguoi choi ko co truong hop dac biet, thi diem se la -13 + maxValue
	else status = -12 + getHighestCard(hand, player); // la bai king thi value = 0
	return status;
}

//ham xep hang nguoi choi
void playersRank(int scores[], int hand[][KCARDS], int rank[], const int players)
{
	for (int index = 0; index < players; index++) // ham tinh diem moi van
		scores[index] = getStatusOfHand(hand, index);

	for (int i = 0; i < players; i++)
		cout << scores[i] << " ";
	cout << endl;
	int check[5];
	for (int i = 0; i < players; i++)
		check[i] = scores[i];
	sort(check, 5);

	for (int i = 0; i < players; i++)
		for (int j = 0; j < players; j++)
			if (check[i] == scores[j])
			{
				rank[i] = j;
			}
	for (int i = 0; i < players; i++)
		cout << rank[i] << " ";
	cout << endl;
}

int playerScore(int rank[], const int scores[], int player, const int players)
{
	for (int i = players - 1; i >= 0; i--)
	{
		if (player == rank[i]) return i;
	}

	
	for (int i = 0; i < players; i++)
	{
		if (scores[i] == scores[player]) player = i;
	}

	for (int i = players - 1; i >= 0; i--)
	{
		if (player == rank[i]) return i;
	}
}