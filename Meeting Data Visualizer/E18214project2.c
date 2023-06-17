/*******************************************************************************************
                    
                AUTHOR      :       H.K. MANAHARA
                SUBJECT     :       CO222
                PROJECT     :       MEETING DATA VISUALIZER - SPECIFICATION
                DATE        :       08/10/2021

*********************************************************************************************/

// IMPORT LIBRARIES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DEFINE STRUCTURE TO STORE DATA OF PEOPLE
typedef struct list
{
    char *name;
    long long int data;
    struct list *next;
} data_set_t;

//***************************** PROTOTYPE FUNCTIONS***********************************
data_set_t *is_name_exits(char *name);
void a_sort();
int indent_length();
int int_len(int num);
void horizontal_bar(int width);
void vertical_bar(int indent);
void pattern_print(int length);
void plot_graph(int indent_value, int graph_length, int is_default);
void print_origin(int indent);
void default_data_sum();

//*****************************GLOBAL VARIABLES***********************************
long long int people_count = 0;
int max_length_word = 0;
int scale = 10;
int graph_length;
int indent;
int graph_length;
int is_default = 0; // 1 for defaul | 0 for not defaul
int scaled = 0;     // 0 if not scaled | 1 if scaled
int wrong_parameter_count = 0;
long long int sum_of_default_data;

//*****************************STRCTURE POINTER VARIABLES***********************************
data_set_t *current;
data_set_t *temp;
data_set_t *head;

//*****************************MAIN FUNCTION***********************************
int main(int argc, char **argv)
{
    FILE *file; //  CREATE A FILE POINTER

    int count_csv = 0; //  A VARIABLE TO COUNT THE .csv FILES
    char want[5];
    int parameter_count = 0;   // A VARIABLE TO COUNT THE PARAMETERS THAT USER INPUTS
    int wrong_files_count = 0; // A VARIABLE TO COUNT THE WRONG FILES
    int line_count = 0;        // A VARIABLE TO COUNT THE LINES OF A FILE
    int empty_line_count = 0;  // A VARIABLE TO COUNT THE EMPTY LINES
    int invalid_files = 0;     //  A VARIABLE TO COUNT THE INVALID FILES

    //  CHECK THE .csv FILES IN THE COMMAND LINE
    for (int i = 1; i < argc; i++)
    {
        if (strstr(argv[i], ".csv"))
        {
            count_csv++; // IF THERE A .csv FILE, INCREASE THE count_csv BY ONE
        }
    }

    char *file_name_array[count_csv]; //  CREATE A ARRAY TO KEEP THE FILE NAMES
    int file_count = 0;               //  A VARIABLE TO COUNT THE FILES

    //*****************************EERROR HANDLING***********************************
    for (int i = 1; i < argc; i++)
    {
        //  IS THERE A .csv FILE INPUTED IN COMMAND LINE, APPEND IT TO THE file_name_array AND INCREASE THE FILE COUNT BY ONE
        if (strstr(argv[i], ".csv")) // CHECK WHETHER THAT ANY COMMAND LINE ARGUMENT HAS .csv EXTENSTION
        {
            //strcpy(file_name_array[file_count], argv[i]);
            file_name_array[file_count] = argv[i];
            file_count++;
        }

        //  IS THERE A ARGUMEN NAMED, --scaled, CHANGE scaled VARIABLE TO 1, (THIS IS VERY IMPORTANT TO CHECK THIS IS A -scaled PLOTTING OR NOT)
        if (strcmp(argv[i], "--scaled") == 0)
        {
            scaled = 1;
        }

        //  CHECK WHETHER THERE IS A -l ARGUMENT AND THERE IS A NUMBER AFTER THAT ARGUMENT
        if (strcmp(argv[i], "-l") == 0)
        {
            if ((i != argc - 1) && atoi(argv[i + 1]))
            {
                scale = atoi(argv[i + 1]); //  IS THERE A ARGUMENT LIKE THAT, ASSIGN IT TO THE SCALE
            }

            //  IF THERE NOT A ARGUMENT LIKE THAT, PRINT THE FOLLOWING ERROR MESSAGE AND EXIT THE PROGRAM
            else
            {
                printf("Not enough options for [-l]\n");
                printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0]);
                exit(0);
            }
        }
        // CHECK WHETHER IS THERE IS A ARGUMENT LIKE THAT, -p, -m OR -t, ASSIGN THAT TO want VARIABLE AND INCREASE THE parameter_count BY ONE
        if ((strcmp(argv[i], "-m") == 0) || (strcmp(argv[i], "-p") == 0) || (strcmp(argv[i], "-t") == 0))
        {
            if ((strlen(want) == 0) || strcmp(argv[i], want) == 0)
            {
                strcpy(want, argv[i]);
                parameter_count++;
            }

            else if ((strlen(want) != 0) || strcmp(argv[i], want) != 0)
            {
                wrong_parameter_count++;
            }
        }

        //  CHECK THAT IS THERE A  INCORRECT  FLAGS IN COMMAND LINE, IS THERE, EXIT THE PROGRAM WITH DISPLAYING MESSAGE
        if ((argv[i][0] == '-') && (!((strcmp(argv[i], "-m") == 0) || (strcmp(argv[i], "-p") == 0) || (strcmp(argv[i], "-t") == 0) || (strcmp(argv[i], "--scaled") == 0) || (strcmp(argv[i], "-l") == 0))))
        {
            printf("Invalid option [%s]\n", argv[i]);
            printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0]);
            exit(0);
        }

        //  CHECK WHETHER THERE IS INVALID FILES GVEN IN COMMAND LINE AS ARGUMENTS
        if ((argv[i][0] != '-') && (!(strstr(argv[i], ".csv"))) && !(atoi(argv[i])))
        {
            invalid_files++;
        }
    }

    //  IF THERE NO PARAMETER IS GIVEN TO PLOT THE GRAPH, SET THE want AS MEETINGS COUNT AND SCALE AS 10
    if (parameter_count == 0)
    {
        strcpy(want, "-m");
        scale = 10;
    }

    //  IF THERE MORE THAN ONE PARAMETERS ARE GIVEN TO PLOT THE GRAPH, PRINTS THE FOLLOWING ERROR MESSAGE AND EXIT FROM THE PROGRAM
    if (wrong_parameter_count > 0)
    {
        printf("Cannot plot multiple parameters in same graph.\n");
        printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0]);
        exit(0);
    }

    //  IF THERE NO ANY .csv FILE, PRINT FOLLOWING ERROR MESSAGE AND EXIT FROM THE PROGRAM
    if (count_csv == 0 || invalid_files > 0)
    {
        printf("Only .csv files should be given as inputs.\n");
        exit(0);
    }

    //  ASSIGN head and current ADDRESSES TO NULL
    head = current = NULL;

    //  GO THEROUGH FOR ALL FILES
    while (file_count != 0)
    {
        file = fopen(file_name_array[file_count - 1], "r"); //  OPEN THE RELEVENT FILE AS READING MODE
        file_count--;                                       // DECREASE THE FILE COUNT BY ONE

        if (file == NULL) //  IS FILE CANNOT BE OPEND, PRINTS THE FOLLOWING MESSAGE AND EXIT FROM THE PROGRAM
        {
            printf("Cannot open one or more given files\n");
            exit(0);
        }

        else // IS FILE OPEN SUCCESSFULLY
        {
            char line[100];

            while (fgets(line, sizeof(line), file)) // GET A LINE OF FILE TO THE line VARIABLE
            {
                if (strlen(line) == 0) //  IF THE LINE IS EMPTY THEN INCREASE THE empty_file_count BY ONE AND BREAK THE LOOP
                {
                    empty_line_count++;
                    break;
                }

                line_count++;

                char *temp = malloc(strlen(line));
                char *piece = strtok(line, ","); //  GET THE FIRST PART TILL , IN THE LINE TO A VAROABLE

                if (strlen(piece) == 0) //  IS THERE IS NO ANY DATA IN FIRST PART IN THE LINE,
                {
                    wrong_files_count++; //  INCREASE THE wrong_files_count BY ONE AND BREAK THE LOOP
                    break;
                }

                data_set_t *exists_element = is_name_exits(piece); //  CHECK WHETHER IS THERE THIS NAME BEFORE

                if (exists_element == NULL) // IF THIS NAME IS NOT EXISTS BEFORE (NEW PERSON)
                {
                    data_set_t *node = malloc(sizeof(data_set_t)); //  CREATE A NEW NODE AND ALLOCATE THE MEMORY FOR NEW NODE
                    node->name = strdup(piece);                    //  ASSIGN THE FIRST PART OF THE LINE TO NEW NODE'S NAME

                    if (strcmp(want, "-m") == 0) //  IF THE WANTED PARAMETER IS -m, THEN
                    {
                        node->data = 1; //   INCREASE THE NODE'S DATA BY ONE
                    }

                    else if (strcmp(want, "-p") == 0) //  IF THE WANTED PARAMETER IS -p, THEN
                    {
                        piece = strtok(NULL, ","); //  GET THE NEXT PIECE OF LINE AS PARTICIPANTS COUNT
                        temp = piece;              //  GET IT TO A TEMPORARY VARIABLE
                        int participants = atoi(temp);

                        node->data = participants; //  ASSIGN THE ABOVE VALUE TO NODE'S DATA
                    }

                    else if (strcmp(want, "-t") == 0) //  IF THE WANTED PARAMETER IS -t, THEN
                    {
                        //  GO THROUGH THE NEXT PART OF LINE AND, SEPERATE IT TO HRS, MINS AND SECS. AFTER THAT, CONVERT ALL OF THEM INTO MINUTES
                        piece = strtok(NULL, ",");

                        piece = strtok(NULL, ":");
                        temp = piece;
                        int hrs = atoi(temp);

                        piece = strtok(NULL, ":");
                        temp = piece;
                        int mins = atoi(temp);

                        piece = strtok(NULL, ":");
                        temp = piece;
                        int secs = atoi(temp);

                        long long int full_time_in_minutes = hrs * 60 + mins + secs / 60; //  FULL TIME IN MINUTES

                        node->data = full_time_in_minutes; //   ASSIGN THE ABOVE DATA TO THE NODE'S DATA
                    }

                    node->next = NULL; //  MAKE THE NEXT LINK TO THE NODE AS NULL

                    //  IF THIS NODE IS THE HEAD NODE, THAT MEANS THERE IS NO NODES BEFORER THAT, CHANGE THE NODE AS HEAD AND CURRENT TO HEAD
                    if (head == NULL)
                    {
                        head = node;
                        current = head;
                    }

                    //  ELSE, MCURRENT-> NEXT MEKE AS THE NODE AND AFTER THAT CURRENT MAKE AS CURRENT->NEXT
                    else
                    {
                        current->next = node;
                        current = current->next;
                    }

                    // IN HERE WE ADDED A NEW PERSON TO THE LIST. SO INCREASE THE people_count BY ONE
                    people_count++;
                }

                else // EXISTS PERSON BEFORE
                {
                    if (strcmp(want, "-m") == 0) //  IF THE WANTED PARAMETER IS -m, THEN
                    {
                        exists_element->data = exists_element->data + 1; //  INCREASE THE EXISTS PERSON'S MEETING COUNT BY ONE
                    }

                    else if (strcmp(want, "-p") == 0) //  IF THE WANTED PARAMETER IS -p, THEN
                    {
                        piece = strtok(NULL, ",");                                  //  GET THE PARTICIPANTS FROM THE LINE USING strtok
                        temp = piece;                                               //  GET IT TO THE TEMPORARY VAROABLE
                        int participants = atoi(temp);                              // CHANGE CONVERT THE STRING VALUE TO INT
                        exists_element->data = exists_element->data + participants; //  ADD CURRENT PARTICIPANTS COUNT TO THE EXISTS PERSON'S PARTICIPANTS
                    }

                    else if (strcmp(want, "-t") == 0) //  IF THE WANTED PARAMETER IS -t, THEN
                    {
                        //  GO THROUGH THE NEXT PART OF LINE AND, SEPERATE IT TO HRS, MINS AND SECS. AFTER THAT, CONVERT ALL OF THEM INTO MINUTES
                        piece = strtok(NULL, ",");
                        //------------------------------------------------------------
                        piece = strtok(NULL, ":");
                        temp = piece;
                        long long int hrs = atoi(temp);

                        piece = strtok(NULL, ":");
                        temp = piece;
                        int mins = atoi(temp);

                        piece = strtok(NULL, ":");
                        temp = piece;
                        int secs = atoi(temp);

                        //----------------------------------------------------------------
                        long long int full_time_in_minutes = hrs * 60 + mins + secs / 60;   //  GET THE FULL TIME IN MINUTES TO A VARIABLE
                        exists_element->data = exists_element->data + full_time_in_minutes; //  ADD CURRENT TIME TO THE EXISTS PERSON'S TIME
                    }
                }
                // new }
            }

            fclose(file); // CLOSE THE FILE
        }
    }

    //  IF ALL LINES ARE EMPTY, PRINTS THIS ERROR MESSAGE AND EXIT
    if (line_count == empty_line_count)
    {
        printf("No data to process\n");
        exit(0);
    }

    //  IF THERE IS NO ANY FILE WITH CORRECT DATA, PRINTS THE FOLLOWING ERROR MESSAGE AND EXIT
    if (count_csv == wrong_files_count)
    {
        printf("File/s contain wrong entries\n");
        exit(0);
    }

    //  IF THE USER GIVES A NUMBER GRATER THAN STUDENT COUNT FOR SCALE, THEN CHANGE IT TO CORRECT VALUE
    if (scale > people_count)
    {
        scale = people_count;
    }

    a_sort(); // CALL THE SORT FUNCTION TO SORT ALL THE DATA

    //  THESE VARIABLES ARE FOR THE PLOTTING
    indent = indent_length();       //  THIS IS THE INDENT LENGTH FROM THE LEFT SIDE, TO THE Y AXIS OF THE GRAPH
    graph_length = 80 - indent - 1; //  THIS IS THE LENGT OF THE X AXIS OF THE GRAPH

    plot_graph(indent, graph_length, scaled); //  PLOT THE GRAPH WITH GIVENN DETAILS

    return 0;
}

//----------------------------------------------------------------END OF MAIN FUNCTION----------------------------------------------------------------

//********************************FUNCTIONS********************************

//  A FUNCTION TO FIND THE INDENT LENGTH
int indent_length()
{
    //  IN THIS FUNCTION GO THROUGH THE ALL NODES WHICH WE HOPE TO PLOT. FIND THE MAXIMUM LENGTH OF THE NAME IN THIS RANGE AND RETURN THE VALUE WHICH ADDED 2 FOR THAT
    int max = 0;

    current = head;
    for (int i = 0; i < scale; i++)
    {
        if (strlen(current->name) > max)
        {
            max = strlen(current->name);
        }
        current = current->next;
    }

    return max + 2; //  RETURN THE INDENT LENGTH FOR THAT GRAPH
}

//----------------------------------------------------------------

//  THIS FUNCTION IS USED TO FINC THE SUM OF ALL DATA IN WHICH IS GAIN FROM THE GIVEN FILE
void default_data_sum()
{
    //  GO THROUGH ALL THE NODES AND ADD ALL OF THE DATA TOGETHER AND STOTR THEM IN sum_of_default_data GLOBAL VARIABLE
    current = head;
    for (int i = 0; i < people_count; i++) // until default scale (=10)
    {
        sum_of_default_data = sum_of_default_data + current->data;
        current = current->next;
    }
}

//----------------------------------------------------------------

//  THIS FUNCTION IS USED TO FIND THE LENGTH OF A INTIGER
int int_len(int num)
{
    int digits = 0;
    while (num != 0)
    {
        num = num / 10;
        digits++;
    }
    return digits;
}

//----------------------------------------------------------------

//  THIS FUNCTION IS USED TO CHECK WHETHER THE GIVEN NAME IS EXISTS BEFORE IN LINKED LIST
data_set_t *is_name_exits(char *name)
{
    data_set_t *temp;                                  //  CREATE A NEW POINTER VARIABLE IN data_set_t DATA TYPE
    for (temp = head; temp != NULL; temp = temp->next) //  GO THROUGH ALL THE NODES WHICH CREATED TILL NOW
    {
        if (strcmp(temp->name, name) == 0) //  IF THERE IS A NODE WITH MATCHED TO THIS NAME
        {
            return temp; // RETURN THIS NODE
        }
    }
    return NULL; // IS THERE NO NAME WHICH MATCHED TO GOVEN NAME RETURN NULL
}

//----------------------------------------------------------------

//  THIS FUNCTION IS TO SORT THE GIVEN  LINKED LIST
void a_sort()
{
    //  CREATE TWO POINTER VARIABLE IN data_set_t DATA TYPE
    data_set_t *temp_1;
    data_set_t *temp_2;

    long long int temp_data; //  THIS VARIABLE IS TO STORE THE DATA OF temp_1, temp_2
    char *temp_name;         //  THIS VARIABLE IS TO STORE THE NAME OF temp_1, temp_2

    //   FOR ALL THE NODES IN LINKED LIST
    for (temp_1 = head; temp_1->next != NULL; temp_1 = temp_1->next)
    {
        //  GO THROUGH ALL THE NODES FROM CURRENT NODE
        for (temp_2 = temp_1->next; temp_2 != NULL; temp_2 = temp_2->next)
        {
            //  IF FIRST DATA IS LESS THAN SECOND DATA, SWAP THE POSISIONS OF THESE DATAs and NAMEs
            if (temp_1->data < temp_2->data)
            {
                temp_data = temp_1->data;
                temp_name = temp_1->name;

                temp_1->data = temp_2->data;
                temp_1->name = temp_2->name;

                temp_2->data = temp_data;
                temp_2->name = temp_name;
            }
        }
    }
}

//----------------------------------------------------------------

//  THIS FUNCTION IS USED TO PLOT HOROZONTAL AXIS OF THE GRAPH FOR GIVEN LENGTH
void horizontal_bar(int width)
{
    for (int i = 0; i < width; i++)
    {
        printf("\u2500");
    }
}

//----------------------------------------------------------------

//  THIS FUNCTION IS USED TO PLOT VERTICAL AXIS OF THE GRAPH FOR GIVEN INDENT
void vertical_bar(int indent)
{ //  PRINT THE SPACES FROM LEFT WITH GIVEN INDENT
    for (int i = 0; i < indent; i++)
    {
        printf(" ");
    }

    //  AFTER THAT PRINTS THE VERTICAL BAR
    printf("\u2502");
}

//----------------------------------------------------------------

//  THIS FUNCTION IS USED TO PLOT THE ORIGIN OF THE GRAPH FOR GIVEN INDENT
void print_origin(int indent)
{
    //  PRINT THE SPACES FROM LEFT WITH GIVEN INDENT
    for (int i = 0; i < indent; i++)
    {
        printf(" ");
    }

    //  AFTER THAT PRINTS THE VERTICAL BAR
    printf("\u2502");

    printf("\n");

    for (int i = 0; i < indent; i++)
    {
        printf(" ");
    }

    //  AFTER THAT PRINTS THE ORIGIN
    printf("\u2514");
}

//----------------------------------------------------------------

//  THIS FUNCTION IS USED TO PRINT THE PATTERN IN BARS IN GRAPH FOR GIVE LENGTH
void pattern_print(int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("\u2591");
    }
}

//----------------------------------------------------------------

//  THIS FUNCTION IS USED TO PLOT THE GRAPH FOR GIVEN DATA
void plot_graph(int indent_value, int graph_length, int is_scaled)
{
    long long int max_value_of_data; //  THIS IS A VARIABLE FOR THE MAX VALUE OF DATA SET TO BE PLOTTED

    if (is_scaled == 0) // IF THIS PLOTTING IS A NON SCALED PLOTTING
    {
        default_data_sum();                      // FIND THE SUM OF ALL GIVEN DATA
        max_value_of_data = sum_of_default_data; // AND AND ASSIGN ABOVE VALUE TO THE max_value_of_data VARIABLE
    }
    printf("\n");

    current = head;
    //  GO THROUTH ALL THE SORTED NODES FOR scale TIMES
    for (int i = 0; i < scale; i++)
    {
        //  IF THIS IS A SCALED PLOTTING, GET THE FIRST NODE'S DATA AS THE max_value_of_data
        if (i == 0 && is_scaled == 1)
        {
            max_value_of_data = current->data;
        }

        //  THIS IS A VARIABLE TO STORE THE REMAIN INDENT (SPACE) FROM THE NAME TO THE VERTICAL BAR
        int remain_indent = indent - strlen(current->name) - 1;

        // PLOT THE VERTICAL BAR AND PATTERN AS THE BELOW
        vertical_bar(indent_value);                                                                          // PRINT THE VERTICAL BAR
        pattern_print((current->data / (float)max_value_of_data) * (graph_length - int_len(current->data))); // PRINT THE PATTERN

        printf("\n");

        printf(" %s", current->name); // PRINTS THE NAME OF PERSON
        vertical_bar(remain_indent);
        pattern_print((current->data / (float)max_value_of_data) * (graph_length - int_len(current->data))); // PRINT THE PATTERN
        printf("%lld", current->data);                                                                       //  PRINTS THE VALUE OF DATA MOSTLY LEFT ON THE BAR

        printf("\n");
        vertical_bar(indent_value);                                                                          // PRINT THE VERTICAL BAR
        pattern_print((current->data / (float)max_value_of_data) * (graph_length - int_len(current->data))); // PRINT THE PATTERN

        printf("\n");
        vertical_bar(indent_value); // PRINT THE VERTICAL BAR
        printf("\n");

        current = current->next; //  MAKE CURRENT AS THE NEXT NODE
    }

    print_origin(indent_value);   //  PRINTS THE ORIGIN
    horizontal_bar(graph_length); // PRINTS THE HORIZONT BAR

    printf("\n");
}

//----------------------------------------------------------------END----------------------------------------------------------------
