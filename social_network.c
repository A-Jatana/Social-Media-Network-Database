#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_STR_LEN 1024

#ifndef MAT_SIZE
#define MAT_SIZE 3 // A small graph
#endif

typedef struct user_struct
{
  char name[MAX_STR_LEN];
  struct friend_node_struct *friends;
  struct brand_node_struct *brands;
  bool visited;
  int distance;
} User;

typedef struct friend_node_struct
{
  User *user;
  struct friend_node_struct *next;
} FriendNode;

typedef struct brand_node_struct
{
  char brand_name[MAX_STR_LEN];
  struct brand_node_struct *next;
} BrandNode;

FriendNode *allUsers = NULL;

int brand_adjacency_matrix[MAT_SIZE][MAT_SIZE];
char brand_names[MAT_SIZE][MAX_STR_LEN];

bool in_friend_list(FriendNode *head, User *node)
{
  for (FriendNode *cur = head; cur != NULL; cur = cur->next)
  {
    if (strcmp(cur->user->name, node->name) == 0)
    {
      return true;
    }
  }
  return false;
}

bool in_brand_list(BrandNode *head, char *name)
{
  for (BrandNode *cur = head; cur != NULL; cur = cur->next)
  {
    if (strcmp(cur->brand_name, name) == 0)
    {
      return true;
    }
  }
  return false;
}

FriendNode *insert_into_friend_list(FriendNode *head, User *node)
{
  if (node == NULL)
    return head;

  if (in_friend_list(head, node))
  {
    printf("User already in list\n");
    return head;
  }

  FriendNode *fn = calloc(1, sizeof(FriendNode));
  fn->user = node;
  fn->next = NULL;

  if (head == NULL)
    return fn;

  if (strcmp(head->user->name, node->name) > 0)
  {
    fn->next = head;
    return fn;
  }

  FriendNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->user->name, node->name) < 0;
       cur = cur->next)
    ;

  fn->next = cur->next;
  cur->next = fn;

  return head;
}

BrandNode *insert_into_brand_list(BrandNode *head, char *node)
{
  if (node == NULL)
    return head;

  if (in_brand_list(head, node))
  {
    printf("Brand already in list\n");
    return head;
  }

  BrandNode *fn = calloc(1, sizeof(BrandNode));
  strcpy(fn->brand_name, node);
  fn->next = NULL;

  if (head == NULL)
    return fn;

  if (strcmp(head->brand_name, node) > 0)
  {
    fn->next = head;
    return fn;
  }

  BrandNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->brand_name, node) < 0;
       cur = cur->next)
    ;

  fn->next = cur->next;
  cur->next = fn;

  return head;
}

FriendNode *delete_from_friend_list(FriendNode *head, User *node)
{
  if (node == NULL)
    return head;

  if (!in_friend_list(head, node))
  {
    printf("User not in list\n");
    return head;
  }

  if (strcmp(head->user->name, node->name) == 0)
  {
    FriendNode *temp = head->next;
    free(head);
    return temp;
  }

  FriendNode *cur;
  for (cur = head; cur->next->user != node; cur = cur->next)
    ;

  FriendNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);

  return head;
}

BrandNode *delete_from_brand_list(BrandNode *head, char *node)
{
  if (node == NULL)
    return head;

  if (!in_brand_list(head, node))
  {
    printf("Brand not in list\n");
    return head;
  }

  if (strcmp(head->brand_name, node) == 0)
  {
    BrandNode *temp = head->next;
    free(head);
    return temp;
  }

  BrandNode *cur;
  for (cur = head; strcmp(cur->next->brand_name, node) != 0; cur = cur->next)
    ;

  BrandNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);

  return head;
}

void print_user_data(User *user)
{
  printf("User name: %s\n", user->name);

  printf("Friends:\n");
  for (FriendNode *f = user->friends; f != NULL; f = f->next)
  {
    printf("   %s\n", f->user->name);
  }

  printf("Brands:\n");
  for (BrandNode *b = user->brands; b != NULL; b = b->next)
  {
    printf("   %s\n", b->brand_name);
  }
}

int get_brand_index(char *name)
{
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (strcmp(brand_names[i], name) == 0)
    {
      return i;
    }
  }

  printf("Brand '%s' not found\n", name);
  return -1; // Not found
}

void print_brand_data(char *brand_name)
{
  int idx = get_brand_index(brand_name);
  if (idx < 0)
  {
    printf("Brand '%s' not in the list.\n", brand_name);
    return;
  }

  printf("Brand name: %s\n", brand_name);
  printf("Brand idx: %d\n", idx);

  printf("Similar brands:\n");
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (brand_adjacency_matrix[idx][i] == 1 && strcmp(brand_names[i], "") != 0)
    {
      printf("   %s\n", brand_names[i]);
    }
  }
}

void populate_brand_matrix(char *file_name)
{
  // Read the file
  char buff[MAX_STR_LEN];
  FILE *f = fopen(file_name, "r");
  fscanf(f, "%s", buff);
  char *line = buff;
  // Load up the brand_names matrix
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (i == MAT_SIZE - 1)
    {
      strcpy(brand_names[i], line);
      break;
    }
    int index = strchr(line, ',') - line;
    strncpy(brand_names[i], line, index);
    line = strchr(line, ',') + sizeof(char);
  }
  // Load up the brand_adjacency_matrix
  for (int x = 0; x < MAT_SIZE; x++)
  {
    fscanf(f, "%s", buff);
    for (int y = 0; y < MAT_SIZE; y++)
    {
      int value = (int)buff[y * 2];
      if (value == 48)
      {
        value = 0;
      }
      else
      {
        value = 1;
      }
      brand_adjacency_matrix[x][y] = value;
    }
  }
}

User *create_user(char *name)
{
	User *new_node;
	new_node = (User *)calloc(1,sizeof(User));

	strcpy(new_node->name, name);
	new_node->friends = NULL;
	new_node->brands =  NULL;
	new_node->distance= 0;
	new_node->visited = false;
	
	bool check = in_friend_list(allUsers, new_node);
	
	if(check == true)
	{
		return NULL;
	}
	
	else
	{
		allUsers = insert_into_friend_list(allUsers, new_node);
		return new_node;
	}
}

int delete_user(User *user)
{
	bool check = in_friend_list(allUsers, user); //check if u use allUsers here
	
	if(check == false)
	{
		return -1;
	}
	
	else
	{
		allUsers = delete_from_friend_list(allUsers, user);
		FriendNode *temp = allUsers;
		while(temp != NULL)
		{
			bool check2 = in_friend_list(temp->user->friends, user);
			if(check2 == true)
			{
				temp->user->friends = delete_from_friend_list(temp->user->friends, user);
			}
			temp = temp->next;
		}
		
		return 0;
	}
}

int add_friend(User *user, User *friend)
{	
	bool check = in_friend_list(user->friends, friend);
	
	if(check == true  || strcmp(user->name, friend->name) == 0)
	{
		return -1;
	}
	
	else
	{
		user->friends = insert_into_friend_list(user->friends, friend);
		friend->friends = insert_into_friend_list(friend->friends, user); //assume friend doesnt have user in their list?		
		return 0;
	}
}

int remove_friend(User *user, User *friend)
{
	bool check = in_friend_list(user->friends, friend);
	
	if(check == false)
	{
		return -1;
	}
	
	else
	{
		user->friends = delete_from_friend_list(user->friends, friend);
		friend->friends = delete_from_friend_list(friend->friends, user); //assume friend doesnt have user in their list?		
		return 0;
	}
}

int follow_brand(User *user, char *brand_name)
{
	bool check = in_brand_list(user->brands, brand_name);
	int invalid = get_brand_index(brand_name);
	
	if(check == true || invalid == -1)
	{
		return -1;
	}
	
	else
	{
		user->brands = insert_into_brand_list(user->brands, brand_name);	
		return 0;
	}
}

int unfollow_brand(User *user, char *brand_name)
{	
	bool check = in_brand_list(user->brands, brand_name);
	int invalid = get_brand_index(brand_name);
	
	if(check == false || invalid == -1)
	{
		return -1;
	}
	
	else
	{
		user->brands = delete_from_brand_list(user->brands, brand_name);	
		return 0;
	}

}

int get_len(FriendNode *head)
{
	FriendNode *b = head;
	int j = 0;
	
	
	while(b != NULL)
	{
		j++;
		b = b->next;
	}
	
	return j;
}

int get_mutual_friends(User *a, User *b)
{
	FriendNode *j = a->friends;
	FriendNode *f = b->friends;
	
	int a_max = get_len(j);
	char arr[a_max][MAX_STR_LEN];
	int i = 0;
	
	while(j != NULL && i < a_max)
	{
		strcpy(arr[i], j->user->name);
		j = j->next;
		i++;
	}
	
	int count = 0;
	
	while(f != NULL)
	{
		for(int h = 0; h < a_max; h++)
		{
			if(strcmp(arr[h], f->user->name) == 0)
			{
				count++;
			}
		}
		f = f->next;
	}
	
	return count;
}

FriendNode *insert_tail(FriendNode *head, User *a)
{
	FriendNode *fn = NULL;
	
	fn = calloc(1, sizeof(FriendNode));
    fn->user = a;
    fn->next = NULL;
	
	if(head == NULL)
	{
		return fn;
	}
	   
	FriendNode *p = NULL;
	   
	p = head;
	
	while(p->next != NULL) 
	{
		p = p->next;
	}
	
	fn->next = p->next;
	p->next = fn;
	
	return head;
}

int helper(User *a, User *b)
{
	
	
    if(strcmp(a->name, b->name) == 0)
    {
        return 1;
    }
    
    return -1;
}
	
int get_degrees_of_connection(User *a, User *b)
{
	
	FriendNode *f = allUsers;
	
	while(f != NULL)
	{
		f->user->visited = false;
		f->user->distance = 0;
		f = f->next;
	}
	
	FriendNode *q = NULL;
    
    int deg = 0;
    
    if(strcmp(a->name, b->name) == 0)
    {
        return deg;
    }
    
    a->visited = true;
    a->distance = 0;
    q = insert_tail(q, a);
    
    
    while(q != NULL)
    {
        User *temp = q->user;
        q = delete_from_friend_list(q, temp);	
		
		FriendNode *t = temp->friends;
        
		
        
        while(t != NULL)
        {
            
            if(t->user->visited == false)
            {
                q = insert_tail(q, t->user);
                
                t->user->distance = temp->distance + 1;
                t->user->visited = true;
                
                int help = helper(t->user, b);
                if(help == 1)
                {
                    return t->user->distance;
                }
            }
            
            t = t->next;
        }
    }
    
    return -1;
	
	
}

void connect_similar_brands(char *brandNameA, char *brandNameB)
{
	int ind_A = get_brand_index(brandNameA);
	int ind_B = get_brand_index(brandNameB);
	
	if(ind_A == -1 || ind_B == -1)
	{
		return;
	}
	
	else
	{
		brand_adjacency_matrix[ind_A][ind_B] = 1;
		brand_adjacency_matrix[ind_B][ind_A] = 1;
		return;
	}
}

void remove_similar_brands(char *brandNameA, char *brandNameB)
{
	int ind_A = get_brand_index(brandNameA);
	int ind_B = get_brand_index(brandNameB);
	
	if(ind_A == -1 || ind_B == -1)
	{
		return;
	}
	
	else
	{
		brand_adjacency_matrix[ind_A][ind_B] = 0;
		brand_adjacency_matrix[ind_B][ind_A] = 0;
		return;
	}
}

int get_brand_len(BrandNode *head)
{
	BrandNode *b = head;
	int j = 0;
	
	
	while(b != NULL)
	{
		j++;
		b = b->next;
	}
	
	return j;
}

int get_mutual_brands(User *a, User *b)
{
	BrandNode *j = a->brands;
	BrandNode *f = b->brands;
	
	int a_max = get_brand_len(j);
	char arr[a_max][MAX_STR_LEN];
	int i = 0;
	
	while(j != NULL && i < a_max)
	{
		strcpy(arr[i], j->brand_name);
		j = j->next;
		i++;
	}
	
	int count = 0;
	
	while(f != NULL)
	{
		for(int h = 0; h < a_max; h++)
		{
			if(strcmp(arr[h], f->brand_name) == 0)
			{
				count++;
			}
		}
		f = f->next;
	}
	
	return count;
}

User *get_suggested_friend(User *user)
{
	FriendNode *headptr = allUsers;
	
	int max = 0;
	int compare;
	User *sugg_friend = NULL;
	
	while(headptr != NULL)
	{
		
		bool check = in_friend_list(user->friends, headptr->user); 
		
		if(strcmp(headptr->user->name, user->name) != 0 && check == false)
		{	
			compare = get_mutual_brands(user, headptr->user);
			if(compare >= max)
			{
				max = compare;
				sugg_friend = headptr->user;
			}
		}
		
		headptr = headptr->next;
		
	}
	
	return sugg_friend;
}

int add_suggested_friends(User *user, int n)
{
	int c = 0;
	User *add = get_suggested_friend(user);
	if(add != NULL)
	{
		int j = add_friend(user, add);
		if(j != -1)
		{
			c++;
		}
	}
	
	while(add != NULL && c < n)
	{
		User *add = get_suggested_friend(user);
		
		if(add == NULL)
		{
			break;
		}
		
		else
		{
			int g = add_friend(user, add);
			if(g != -1)
			{
				c++;
			}
		}
	}
	
	return c;
}		

char follow[MAT_SIZE][MAX_STR_LEN];
char sugg_brand[MAT_SIZE][MAX_STR_LEN];

int get_follow_index(char array[MAT_SIZE][MAX_STR_LEN], char *name)
{
	for (int i = 0; i < MAT_SIZE; i++)
	{
		if (strcmp(array[i], name) == 0)
		{
			return 3;
		}
	}
  
	return -1; 
}

char *get_suggested_brand(User *user)
{
	int one;
	int two;
	BrandNode *ptr = user->brands;
	
	int sim_brand[MAT_SIZE] = {0};
	
	strcpy(sugg_brand[0], " "); 
	
	if(ptr == NULL)
	{
		for(int i = 0; i < MAT_SIZE; i++)
		{
			int check_emp = get_follow_index(follow, brand_names[i]);
			
			if(check_emp == -1)
			{
				if(strcmp(brand_names[i], sugg_brand[0]) > 0)
				{
					strcpy(sugg_brand[0], brand_names[i]);
				}
			}
		}
		
		char *holder = sugg_brand[0];
		return holder;
	}
	
	else
	{
		for(int i = 0; i < MAT_SIZE; i++)
		{
			bool check = in_brand_list(user->brands, brand_names[i]);
			int check2 = get_follow_index(follow, brand_names[i]);
	
			if(check == false && check2 == -1)
			{	
				ptr = user->brands;
				while(ptr != NULL)
				{
					one = get_brand_index(ptr->brand_name);
					two = get_brand_index(brand_names[i]);
	
					if(brand_adjacency_matrix[one][two] == 1 && brand_adjacency_matrix[two][one] == 1)
					{
						sim_brand[i] += 1;
					}
					
					ptr = ptr->next;
				}		
			}
		}
	}
	
	int h = 0;
	int max = 0;
	
	while(h != MAT_SIZE)
	{
		if(sim_brand[h] > max)
		{
			strcpy(sugg_brand[0], brand_names[h]);
			max = sim_brand[h];
		}
		
		if(sim_brand[h] == max)
		{
			if(strcmp(brand_names[h], sugg_brand[0]) > 0)
			{
				strcpy(sugg_brand[0], brand_names[h]);
			}
		}
		h++;
	}
		
	
	char *holder = sugg_brand[0];

	return holder;
}

int follow_suggested_brands(User *user, int n)
{
	if(user == NULL)
	{
		return 0;
	}
	
	int c = 0;
	
	for (int i = 0; i < MAT_SIZE; i++)
	{
		strcpy(follow[i], " ");
	}

	for(int j = 0; j < n; j++)
	{			
		strcpy(follow[j], get_suggested_brand(user));
		c++;
			
		if(strcmp(follow[j], " ") == 0)
		{
			c = c - 1;
			break;
		}
		
	}
	
	for(int f = 0; f < c; f++)
	{
		follow_brand(user, follow[f]);
	}
	
	return c;
}		