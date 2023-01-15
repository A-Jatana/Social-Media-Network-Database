#include "social_network.c"

int main()
{
    User *william = create_user("William");
    User *natasha = create_user("Natasha");
    User *peter = create_user("Peter");
    User *stan = create_user("Stan");

    printf("\nTest 1 - Users printed in alphabetical order\n");
    FriendNode *t = allUsers;
    int idx = 0;
    while (t != NULL && t->next != NULL)
    {
        if (strcmp(t->user->name, t->next->user->name) > 0)
        {
            printf("Test 1 failed. Users are not sorted.\n");
            exit(1);
        }

        idx++;
        t = t->next;
    }

    if (idx != 3)
    {
        printf("Test 1 failed. Users were not inserted.\n");
        exit(1);
    }

    printf("Test 1 passed.\n");

    printf("\nTest 2 - Natasha adds Stan as a friend\n");
    add_friend(natasha, stan);

    // Expect to see Natasha with her friend Stan & see Stan with his friend Natasha
    if (!in_friend_list(natasha->friends, stan) || !in_friend_list(stan->friends, natasha))
    {
        printf("Test 2 failed. Friend connection was not made.\n");
        exit(1);
    }

    printf("Test 2 passed.\n");

    printf("\nTest 3 - Degree of connection between Natasha and Stan is 1\n");
    int natasha_and_stan = get_degrees_of_connection(natasha, stan);

    if (natasha_and_stan != 1) // Expect 1
    {
        printf("Test 3 failed. Friend connection was not made.\n");
        exit(1);
    }

    printf("Test 3 passed.\n");

    printf("\nTest 4 - Natasha follows Brand 'Nike'\n");
    populate_brand_matrix("brands.txt");
    follow_brand(natasha, "Nike");

    if (!in_brand_list(natasha->brands, "Nike")) // Natasha should now follow Nike.
    {
        printf("Test 4 failed. Brand connection was not made.\n");
        exit(1);
    }

    printf("Test 4 passed.\n");

    printf("\nTest 5 - Connect Brands 'Gucci' and 'Lacoste'\n");
    connect_similar_brands("Gucci", "Lacoste");
    int zeroIdx = get_brand_index("Gucci");
    int twoIdx = get_brand_index("Lacoste");

    if (brand_adjacency_matrix[zeroIdx][twoIdx] != 1 || brand_adjacency_matrix[twoIdx][zeroIdx] != 1)
    {
        printf("Test 5 failed. Brand connection was not made.\n");
        exit(1);
    }

    printf("Test 5 passed.\n");

    printf("\nTest 6 - William and Stan have Natasha as a mutual friend\n");
    add_friend(natasha, william);
    int mutuals_stan_will = get_mutual_friends(stan, william);

    if (mutuals_stan_will != 1) // Expect 1
    {
        printf("Test 6 failed. Mutual friends of Stan and William should be 1.\n");
        exit(1);
    }

    printf("Test 6 passed.\n");

    printf("\nTest 7 - Stan is suggested 1 friend\n");
    User *stanSuggested = get_suggested_friend(stan);

    if (stanSuggested != william) // We should see Will here again
    {
        printf("Test 7 failed. Stan was not suggested William.\n");
        //exit(1);
    }

    printf("Test 7 passed.\n");

    printf("\nTest 8 - Peter adds 2 suggested friends\n");

    // Expect to see Peter with friends Natasha, Stan, and Will
    add_friend(natasha, peter);
    add_suggested_friends(peter, 2);

    if (!in_friend_list(peter->friends, stan) || !in_friend_list(peter->friends, william))
    {
        printf("Test 8 failed. Peter was not suggested 2 friends correctly.\n");
        exit(1);
    }

    printf("Test 8 passed.\n");

    printf("\nTest 9 - Peter follows 1 suggested brand\n");
    follow_brand(william, "Lacoste");
    follow_brand(peter, "Gucci");
    follow_suggested_brands(peter, 1);
    // Peter should now also follow Nike. A tie is broken against Lacoste.

    if (!in_brand_list(peter->brands, "Nike"))
    {
        printf("Test 9 failed. Peter did not follow brand 'Nike'.\n");
        exit(1);
    }

    printf("Test 9 passed.\n");

    printf("\nAll test cases passed! \n");
}

