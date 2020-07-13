#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

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
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
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
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    int s = candidate_count;

    for (int n = 0; n < candidate_count; n++, s--)
    {
        for (int k = 1; k < s; k++)
        {
            preferences[ranks[n]][ranks[n + k]] += 1;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int n = 0; n < candidate_count; n++)
    {
        for (int k = n + 1; k < candidate_count; k++)
        {
            if (preferences[n][k] > preferences[k][n])
            {
                pairs[pair_count].winner = n;
                pairs[pair_count].loser = k;
                pair_count++;
            }
            else if (preferences[n][k] < preferences[k][n])
            {
                pairs[pair_count].winner = k;
                pairs[pair_count].loser = n;
                pair_count++;
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int k = 0; k < pair_count - 1; k++)
    {
        int a = preferences[pairs[k].winner][pairs[k].loser];
        int b = preferences[pairs[k].loser][pairs[k].winner];
        int diff1 = a - b;

        for (int s = k + 1; s < pair_count; s++)
        {
            int c = preferences[pairs[s].winner][pairs[s].loser];
            int d = preferences[pairs[s].loser][pairs[s].winner];
            int diff2 = c - d;
            if (diff1 < diff2)
            {
                pair temp = pairs[k];
                pairs[k] = pairs[s];
                pairs[s] = temp;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    //set all edges on graph.
    for (int s = 0; s < pair_count; s++)
    {
        int n = pairs[s].winner;
        int m = pairs[s].loser;
        locked[n][m] = true;
    }

    //remove edges that cause a cycle. a cycle is formed when pairs[s].winner == pairs[t].loser
    for (int s = 0; s < pair_count; s++)
    {
        for (int t = s + 1; t < pair_count; t++)
        {
            if (pairs[s].winner == pairs[t].loser)
            {
                int n = pairs[t].winner;
                int m = pairs[t].loser;
                locked[n][m] = false;
            }
        }
    }
    return;
}













// Print the winner of the election
void print_winner(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        int count = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j] == false)
            {
                count++;
            }
            if (count == candidate_count)
            {
                printf("%s\n", candidates[pairs[i].winner]);
            }
        }
    }
    return;
}