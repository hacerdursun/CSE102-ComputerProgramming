#include<stdio.h>

struct Customer
{
    char Name[50]; //I limited the name value to 50 characters
    int Age;
    int Label;
};

//I defined the limits as constant
const int EMPLOYEE_LIMIT = 5;
const int RETIRED_LIMIT = 3;
const int STUDENT_LIMIT = 3;
const int UNEMPLOYED_LIMIT = 2;
const int OTHER_LIMIT = 2;
const int CUSTOMER_LIMITS[] = {5, 3, 3, 2, 2};

//Calculates the number of times in a sequence from the customer with the label number given as the parameter
int findCustomerLabel(struct Customer *customers, int *customer_index, int label)
{
    int i, repeat = 0;
    for(i = 0; i <= (*customer_index); i++)
	{
        if(customers[i].Label == label)
		{
            while((customers[i].Label == label) && (i <= (*customer_index)))
			{
                i++;
                repeat++;
            }
            break;
        }
    }
    
    return repeat;
}

int addCustomer(struct Customer *customers, int *customer_index)
{
	int i, j;
    if(*customer_index == 49) //Because the maximum capacity is 50, the index can be at most 49
	{ 
        printf("\nTotal customer process capacity is full.\n");
        return 0;
    }
    else
	{
        struct Customer new_customer;
        getchar(); // \n so I used it to skip the new_line character
        printf("Please enter the name of the customer : ");
        fgets(new_customer.Name, sizeof(new_customer.Name), stdin);
        //scanf("%[^\n]", &(new_customer.Name));
        printf("Please enter the age of the customer : ");
        scanf("%d", &(new_customer.Age));
        printf("Please enter the label of the customer : ");
        scanf("%d", &(new_customer.Label));
        if(new_customer.Label < 0 || new_customer.Label > 4)
		{
            printf("\nPlease enter the label between 0-4!!\n");
            return 0;
        }
        int label_total = findCustomerLabel(customers, customer_index, new_customer.Label);
        //If there is no customer in line
        if(label_total == 0)
		{
            int added = 0; //to keep whether it has been added or not
            for(i = 0; i <= (*customer_index); i++)
			{
                //If it is a priority customer, it will be added to the sequence
                if(customers[i].Label > new_customer.Label)
				{
                    /*The loop will run from the end of the sequence to the point where it is desired to be added, and 
					each customer will be moved one step further*/
                    for(j = (*customer_index); j >= i; j--)
					{
                        customers[j + 1] = customers[j];
                    }
                    customers[i] = new_customer; //Adding process
                    added = 1; //For added information
                    (*customer_index)++; //If added, index is incremented by one 
                    break;
                }
            }
            //If there is a customer who has reached the limit that has to come consecutively, add it after the customer
            if(added == 0)
			{
                int label_limit = 0, pass_limit = 0;
                for(i = 0; i <= (*customer_index); i++)
				{
                    label_limit = findCustomerLabel(customers, customer_index, customers[i].Label);
                    //If the customer has exceeded the limit that must come consecutively
                    if(label_limit > CUSTOMER_LIMITS[customers[i].Label])
					{
                        pass_limit = 1;
                        //I are passing the limit of coming in a row
                        while(pass_limit <= CUSTOMER_LIMITS[customers[i].Label])
						{
                            pass_limit++;
                            i++;
                        }
                        //Additions can now be made here
                        for(j = (*customer_index); j >= i; j--)
						{
                            customers[j + 1] = customers[j];
                        }
                        customers[i] = new_customer; //Adding process
                        added = 1; //For added information
                        (*customer_index)++; //If added, index is incremented by one
                        break;
                    }
                }
            }
            //It adds to the end if it has not been added yet
            if(added == 0)
			{
                (*customer_index)++; //First I need to increase the index and then add
                customers[*customer_index] = new_customer; //Adding to the end
                added = 1; 
            }
        }
        else
		{ 
			//If there is this type of customer at the current sequence
            //If the consecutive limit of the customer is reached, add to the end
            int add_to_end = 0;
            if(new_customer.Label == 0 && label_total == EMPLOYEE_LIMIT)
                add_to_end = 1;
            else if(new_customer.Label == 1 && label_total == RETIRED_LIMIT)
                add_to_end = 1;
            else if(new_customer.Label == 2 && label_total == STUDENT_LIMIT)
                add_to_end = 1;
            else if(new_customer.Label == 3 && label_total == UNEMPLOYED_LIMIT)
                add_to_end = 1;
            else if(new_customer.Label == 4 && label_total == OTHER_LIMIT)
                add_to_end = 1;
            //If it is to be added to the end
            if(add_to_end == 1)
			{
                (*customer_index)++; //First I need to increase the index and then add
                customers[*customer_index] = new_customer; //Adding to the end
            }
            else
			{ 
				//If it is to be added to the sequence
                for(i = 0; i <= (*customer_index); i++)
				{
                    //There is a location that needs to come in a row
                    if(customers[i].Label == new_customer.Label)
					{
                        //Move to the position that needs to be add
                        while(customers[i].Label == new_customer.Label)
						{
                            i++;
                        }
                        for(j = (*customer_index); j >= i; j--)
						{
                            customers[j + 1] = customers[j];
                        }
                        customers[i] = new_customer; //Adding process
                        (*customer_index)++;
                        break;
                    }
                }
            }
        }
    }
    
    return 1;
}

int processCustomer(struct Customer *customers, int *customer_index)
{
	int i;
    if(*customer_index == -1) //If no one is next
	{ 
        printf("\nThere is not any customer in bank system sequence.\n");
        return 0;
    }
    else
	{
        printf("Proceed customer is %s\n", customers[0].Name);
        //This loop removes the first person in the sequence, moving each customer one step forward in the sequence
        for(i = 0; i <= (*customer_index - 1); i++)
		{
            customers[i] = customers[i + 1];
        }
        //I reduce the total number of customers by 1 after the transaction
        (*customer_index)--;
        return 1;
    }
}

void printSequence(struct Customer *customers, int *customer_index)
{	
	int i;
    for(i = 0; i <= (*customer_index); i++)
	{
        printf("%d", customers[i].Label);
        if(i != (*customer_index))
		{
            printf("-");
        }
    }
    printf("\n");
}

int main()
{
    struct Customer customers[50]; //Up to 50 customers
    int customer_index = -1; //Stores the total number of customers as an index, so if there is 1 customer, the index will be 0
    int choose = 0; 
    int total_customer_process = 0; //When it reaches 50, the program will end 
    printf("***********Banking System***********\n");
    do
    {
        printf("\nCurrent Sequence: ");
        printSequence(customers, &customer_index);
        printf("\n1- Add customer\n");
        printf("\n2- Process customer\n");
        scanf("%d", &choose);
        if(choose == 1)
		{
            if(addCustomer(customers, &customer_index) == 1)
                total_customer_process++;
        }
        else if(choose == 2)
		{
            if(processCustomer(customers,&customer_index) == 1)
                total_customer_process++;
        }
        if(total_customer_process == 50)
		{
            printf("\nCustomer processing capacity is full, the program will be terminated.\n");
            break;
        }

    }while(1);
    
    return 0;
}
