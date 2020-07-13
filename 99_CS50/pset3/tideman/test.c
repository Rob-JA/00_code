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