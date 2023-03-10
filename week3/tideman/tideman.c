#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// ABS Macro
#define ABS(x, y)   x >= y ? x - y : y - x

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }

    // Init the arrays
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Find the candidate in the candidates array and fill in ranks[]
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
// 3 0 4 1 2
// 3 > 0, 4, 1, 2 -----> pre[3][0]++, pre[3][4]++, ....
// 0 > 4, 1, 2
// 4 > 1, 2
// 1 > 2
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
}

void    swap_int(int *a, int *b)
{
    int temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

void    swap_pair(pair *a, pair *b)
{
    pair    temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int arr[pair_count];
    int winner;
    int loser;

    // Get the strength of victory between the winner and losers of all pairs
    for (int i = 0; i < pair_count; i++)
    {
        winner = pairs[i].winner;
        loser = pairs[i].loser;
        arr[i] = preferences[winner][loser] - preferences[loser][winner];
    }
    // Bubble sort
    int i = 0;
    while (i < pair_count - 1)
    {
        if (arr[i] < arr[i + 1])
        {
            swap_int(&arr[i], &arr[i + 1]);
            swap_pair(&pairs[i], &pairs[i + 1]);
            i = 0;
        }
        i++;
    }
}

bool    find_cycle(int winner, int loser, int temp)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[temp][i] == true)
        {
            if (i == winner)
            {
                return (true);
            }
            else if (find_cycle(winner, loser, i) == true)
            {
                return (true);
            }
        }
    }
    return (false);
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;
        if (find_cycle(pairs[i].winner, pairs[i].loser, pairs[i].loser) == true)
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }
}

// Print the winner of the election
void print_winner(void)
{
    int true_count;
    int winner;

    winner = -1;
    for (int i = 0; i < candidate_count; i++)
    {
        true_count = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                true_count++;
            }
        }
        if (true_count == 0)
        {
            winner = i;
            break ;
        }
    }
    printf("%s\n", candidates[winner]);
}