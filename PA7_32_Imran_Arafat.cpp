#include <cstdio>
#include <queue>
#include<stack>
#include <vector>
#include<string.h>
#include<ctype.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include <iostream>
using namespace std;
stack <char> my_stack;
char items[50][100][100];
char new_items[50][100][100];
char grammar[20][10],nonTer[100]= {0},ter[100]= {0},firstset[20][20],tempfirst[20],followset[20][20],temparray[20];
int nnt=0,nt=0,n,arrayind=0,parse_table[50][50]= {-1},no_of_states = 0, no_of_items[30];
int state_index = 0, goto_state_index = 0, closure_item_index = 0;
int number_of_nonterminal, number_of_terminal;
char expression[100];

bool LL1=true;

int epsilon(char ch)
{
    int i;
    for(i=0; i<n; i++)
    {
        if(ch==grammar[i][0])
        {
            if(grammar[i][2]=='#')
                return 1;
        }
    }
    return 0;
}
int isterminal(char ch)
{
    int i;

    for(i = 0; i < strlen(ter); i++)
        if(ch == ter[i])
            return 1;

    return 0;
}
int isnonterminal(char ch)
{
    int i;

    for(i = 0; i < strlen(nonTer); i++)
        if(ch == nonTer[i])
            return 1;

    return 0;
}
int nonTerminal_no(char ch)
{
    int i=0;
    for(i=0; i<strlen(nonTer); i++)
    {
        if(nonTer[i]==ch)
            return i;
    }

}
int Terminal_no(char ch)
{
    int i=0;
    for(i=0; i<strlen(ter); i++)
    {
        if(ter[i]==ch)
            return i;
    }

}
char Result[100];
void addToResultSet(char Result[],char val)
{
    int k;
    for(k=0 ; Result[k]!='\0'; k++)
        if(Result[k]==val)
            return;
    Result[k]=val;
    Result[k+1]='\0';
}

void FOLLOWcalc(char ch)
{
    int i,j,k,l,m,nonT_no;

    Result[0]='\0';
    if(ch==grammar[0][0])
        addToResultSet(Result,'$');
    for(i=0; i<n; i++)
    {
        int length;
        length=strlen(grammar[i]);
        for(j=2; j<length; j++)
        {
            if(grammar[i][j]==ch)
            {
                if(grammar[i][j+1]!='\0' && !(isupper(grammar[i][j+1])))
                {
                    addToResultSet(Result,grammar[i][j+1]);
                }
                if(grammar[i][j+1]!='\0' && (isupper(grammar[i][j+1])))
                {
                    nonT_no=nonTerminal_no(grammar[i][j+1]);
                    int found_ep=0;
                    for(k=0; k<strlen(firstset[nonT_no]); k++)
                    {
                        if((firstset[nonT_no][k])=='#')
                        {
                            found_ep=1;
                        }
                        else
                        {
                            addToResultSet(Result,firstset[nonT_no][k]);
                        }
                    }
                    if(found_ep==1)
                    {
                        while(found_ep==1)
                        {
                            j++;
                            if(j>length)
                                break;
                            int newvar=0;
                            if(grammar[i][j+1]!='\0' && !(isupper(grammar[i][j+1])))
                            {
                                addToResultSet(Result,grammar[i][j+1]);
                            }

                            else if(grammar[i][j+1]!='\0' && (isupper(grammar[i][j+1])))
                            {
                                nonT_no=nonTerminal_no(grammar[i][j+1]);

                                for(k=0; k<strlen(firstset[nonT_no]); k++)
                                {
                                    if((firstset[nonT_no][k])!='#')
                                    {
                                        addToResultSet(Result,firstset[nonT_no][k]);
                                    }
                                    else
                                    {
                                        newvar=1;
                                    }
                                }
                                if(newvar==0)
                                    found_ep=0;
                            }
                            else if(grammar[i][j+1]=='\0' && (isupper(grammar[i][j])))
                            {
                                nonT_no=nonTerminal_no(grammar[i][0]);
                                for(k=0; k<strlen(followset[nonT_no]); k++)
                                {

                                    addToResultSet(Result,followset[nonT_no][k]);
                                }
                                int dec=0;
                                for(k=0; k<strlen(firstset[nonT_no]); k++)
                                {

                                    if((firstset[nonT_no][k])=='#')
                                    {
                                        dec=1;
                                    }
                                }
                                if(dec==0)
                                    found_ep=0;
                            }
                        }
                    }
                }
                if(grammar[i][j+1]=='\0' && (isupper(grammar[i][j])))
                {
                    nonT_no=nonTerminal_no(grammar[i][0]);
                    for(k=0; k<strlen(followset[nonT_no]); k++)
                    {
                        addToResultSet(Result,followset[nonT_no][k]);
                    }
                }
            }
        }
    }
}


void FIRSTcalc(char* Result,char c,char* rule_r)
{
    int i,j,k;
    char subResult[20];
    int foundEpsilon;
    subResult[0]='\0';
    Result[0]='\0';
    if(!(isupper(c)))
    {
        addToResultSet(Result,c);
        return ;
    }
    for(i=0; i<n; i++)
    {
        if(strcmp(rule_r,grammar[i]))
        {
            if(grammar[i][0]==c)
            {
                if(grammar[i][2]=='#')
                    addToResultSet(Result,'#');
                else if(grammar[i][2]!=c)
                {
                    j=2;
                    while(grammar[i][j]!='\0')
                    {
                        foundEpsilon=0;
                        FIRSTcalc(subResult,grammar[i][j],grammar[i]);
                        for(k=0; subResult[k]!='\0'; k++)
                            addToResultSet(Result,subResult[k]);
                        for(k=0; subResult[k]!='\0'; k++)
                            if(subResult[k]=='#')
                            {
                                foundEpsilon=1;
                                break;
                            }
                        if(!foundEpsilon)
                            break;
                        j++;
                    }
                }
            }
        }
    }
    return ;
}

void make_terminal_nonterminal()
{
    int i,j,k,l,m,a,b;
    for(i=0; i<n; i++)
    {
        l=strlen(grammar[i]);
        for(j=0; j<l; j++)
        {
            if(isupper(grammar[i][j]))
            {
                a=0;
                for(k=0; k<strlen(nonTer); k++)
                {
                    if(grammar[i][j]==nonTer[k])
                    {
                        a=1;
                    }
                }
                if(a==0)
                {
                    nonTer[nnt]=grammar[i][j];
                    nnt++;
                }
            }
            else if(grammar[i][j]!='=')
            {
                b=0;
                for(k=0; k<strlen(ter); k++)
                {
                    if(grammar[i][j]==ter[k])
                    {
                        b=1;
                    }
                }
                if(b==0)
                {
                    ter[nt]=grammar[i][j];
                    nt++;
                }
            }
        }
    }
}

void generate_item(char *s, char *t, char lookah[10])
{
    int i,j;
    for(i = 0; i < 2; i++)
        t[i] = s[i];

    t[i] = '.';

    if(s[i] != ',')
        for(; i < strlen(s); i++)
            t[i+1] = s[i];

    t[i+1] = ',';
    for(i=i+2,j=0; j<strlen(lookah); i++,j++)
    {
        t[i]=lookah[j];
    }
    t[i]='\0';
}


int item_found(char *s)  	//Check for items in a state.
{
    int i;

    for(i = 0; i < closure_item_index; i++)
    {
        if(!strcmp(s, items[state_index][i]))	//If the strings match.
            return 1;
    }

    return 0;
}

void closure(char *s)
{
    int i, j,fake;
    int kk,kk1,kk2,kk3,q1,q2,q3;
    q3=strlen(s);
    if(!(isterminal(s[q3-1])) &&s[q3-1]!='$')
        s[q3-1]='\0';
    char lookahed[10],newlook[10];
    for(i = 0; s[i] != '.'; i++);
    i++;
    if(!item_found(s))
    {
        strcpy(items[state_index][closure_item_index], s);
        closure_item_index++;
    }
    if(s[i] == s[0] && s[i-2] == '=')	//To avoid infinite loop due to left recursion.
        return;

    if(isterminal(s[i]))
        return;
    else 	 	//Not a terminal
    {
        for(j = 0; j < n; j++)
        {
            char temp[100];

            if(grammar[j][0] == s[i])
            {
                if(isterminal(s[i+1]))
                {
                    lookahed[0]=s[i+1];

                    lookahed[1]='\0';
                }

                else if(j==0)
                {
                    lookahed[0]='$';
                    lookahed[1]='\0';
                }
                else if(s[i+1]==',')
                {
                    int xp=strlen(s)-1;
                    for(xp-1,kk=0; s[xp]!=','; xp--)
                    {

                        if(isterminal(s[xp]) || s[xp]=='$')
                        {
                            newlook[kk]=s[xp];
                            kk++;
                        }

                    }
                    newlook[kk]='\0';
                    kk=strlen(newlook);

                    int news,look=kk-1;
                    for(news=0; news<kk; news++,look--)
                    {
                        lookahed[news]=newlook[look];
                    }
                    lookahed[kk]='\0';
                }
                else
                {
                    int z=nonTerminal_no(s[i+1]);
                    {
                        for(kk1=0,kk3=0; kk1<strlen(firstset[z]); kk1++)
                        {
                            if(firstset[z][kk1]=='#')
                            {
                                q1=0;
                                while(s[q1]!=',')
                                {
                                    q1++;

                                }
                                q1++;
                                while(s[q1]!='\0')
                                {
                                    lookahed[kk3]=s[q1];
                                    kk3++;
                                    q1++;
                                }
                            }
                            else
                                lookahed[kk3]=firstset[z][kk1];
                            kk3++;
                        }

                    }
                    lookahed[kk3]='\0';

                }
                generate_item(grammar[j], temp,lookahed);

                closure(temp);
            }
        }
    }
}


void initialize_items()
{
    int i;

    for(i = 0; i < 30; i++)
    {
        no_of_items[i] = 0;
    }
}
int Goto1(char s, char temp[][100])  	//Find Goto on symbol s. GOTO(goto_state_index, s)
{
    int i, j;
    int np = 0;
    char t, temp2[100];

    //if(s == ',' || s=='#')
    if(s == ',' || s=='#')
    {
        return np;
    }

    for(i = 0; i < no_of_items[goto_state_index]; i++)
    {
        strcpy(temp2, items[goto_state_index][i]);

        for(j = 0; temp2[j] != '.'; j++);
// if(temp2[j+1] == ',' || temp2[j+1]=='#')
        if(temp2[j+1] == ',')
            continue;

        if(temp2[j+1] == s)
        {
            t = temp2[j];
            temp2[j] = temp2[j+1];
            temp2[j+1] = t;

            strcpy(temp[np], temp2);
            np++;
        }
    }

    return np;
}
int transition_item_found(char * t_items, char s, int t_index)
{
    int i;

    for(i = 0; i < t_index; i++)
        if(s == t_items[i])
            return 1;

    return 0;
}
int state_found(char *s)  	//Checks for existance of same state.
{
    int i;

    for(i = 0; i < state_index; i++)
    {
        if(!strcmp(s, items[i][0]))	//Compare with the first item of each state.
            return 1;
    }

    return 0;
}
void compute_closure_goto()
{
    char temp[100][100], transition_items[100],lookahed[10];;
    int i, no_of_goto_items,j, transition_index = 0;

    lookahed[0]='$';
//temp[0][strlen(temp[0])]='\0';

    generate_item(grammar[0], temp[0],lookahed);

    lookahed[0]='$';

    closure(temp[0]);

    no_of_items[state_index] = closure_item_index;
    closure_item_index = 0;

    state_index++;

    while(goto_state_index < 30)
    {
        transition_index = 0;
        transition_items[transition_index] = ',';

        for(i = 0; i < no_of_items[goto_state_index]; i++)
        {
            for(j = 0; items[ goto_state_index][i][j] != '.'; j++);
            j++;

            if(!transition_item_found(transition_items, items[goto_state_index][i][j], transition_index))
            {
                transition_items[transition_index] = items[goto_state_index][i][j];
                transition_index++;
            }
        }
        transition_items[transition_index] = ',';

        for(i = 0; i < transition_index; i++)
        {
            int add_flag = 0;

            no_of_goto_items = Goto1(transition_items[i], temp);

            for(j = 0; j < no_of_goto_items; j++)
            {
                if(!state_found(temp[j]))
                {
                    add_flag = 1;
                    closure(temp[j]);
                }
                else
                    break;
            }
            if(add_flag)
            {
                no_of_items[state_index] = closure_item_index;
                closure_item_index = 0;
                state_index++;
            }
        }
        goto_state_index++;
    }
    no_of_states = state_index;
}
void print()
{
    int i, j;

    printf("\nNumber of states = %d.\n", no_of_states);

    for(i = 0; i < no_of_states; i++)
    {
        printf("\n\nItems in State %d...\n\n", i);

        for(j = 0; j < no_of_items[i]; j++)
            printf("%s\n", items[i][j]);
    }
}
int tableLR1[30][30];
int tableLALR[30][30];
void table_initaialize()
{
    int i,j,k,l,m,n,goto_column,action_column;
    action_column=strlen(ter)+1;
    goto_column=strlen(nonTer)-1;
    for(i=0; i<no_of_states; i++)
    {
        for(j=0; j<20; j++)
        {
            tableLR1[i][j]=0;
        }
    }
}

int item_search(char sr[100])
{
    int i,j,k,l,m,n;
    for(i=0; i<no_of_states; i++)
    {
        for(j=0; j<no_of_items[i]; j++)
        {

            if(!strcmp(items[i][j],sr))
            {

                return i;
            }
        }
    }
    return 500;
}
int isSame (char* a, char* b)
{

    int len_a,len_b,i,j,k;
    len_a=strlen(a);
    len_b=strlen(b);
    if(len_a==len_b)
    {
        for(i=0; i<len_a; i++)
        {
            if(a[i]!=b[i])
                return 0;
        }
    }
    return 1;
}

void entry_table()
{
    int k,i,j,l,dot_pos,goto_no,nont_no,t_no,action_no,reduced_rule;
    char sr[100],sub_sr[100],ch,ch2;
    number_of_nonterminal=strlen(nonTer);
    number_of_terminal=strlen(ter);
    int epsi_k,epsi_k1,epsi_k2,epsi_k3;
    char eps_red[100];
    int epsilon_ruel_reduce;
    for(i=0; i<no_of_states; i++)
    {
        for(j=0; j<no_of_items[i]; j++)
        {
            strcpy(sr,items[i][j]);
            for(k=0; sr[k]!='.'; k++);
            dot_pos=k;//;sr.find(".");
            ch2=sr[dot_pos+1];
            if(sr[dot_pos+1]=='#')
            {
                for(epsi_k2=0,epsi_k3=0; sr[epsi_k2]!=','; epsi_k2++,epsi_k3)
                {
                    if(sr[epsi_k2]!='.')
                    {
                        eps_red[epsi_k3]=sr[epsi_k2];
                        epsi_k3++;
                    }

                }
                eps_red[epsi_k3]='\0';
                for(epsi_k3=1; epsi_k3<n; epsi_k3++)
                {
                    int asdf;
                    asdf=strcmp(eps_red,grammar[epsi_k3]);
                    if(asdf==0)
                    {
                        epsilon_ruel_reduce=epsi_k3;
                    }
                }
                {
                    for(epsi_k1=dot_pos+3; sr[epsi_k1]!='\0'; epsi_k1++)
                    {
                        if(isterminal(sr[epsi_k1]))
                        {

                            tableLR1[i][Terminal_no(sr[epsi_k1])]=0-epsilon_ruel_reduce;
                        }
                        else
                            tableLR1[i][number_of_terminal]=0-epsilon_ruel_reduce;
                    }
                }
            }
            ///not rule reduced
            if(sr[dot_pos+1]!=',')
            {
                ch=sr[dot_pos];
                sr[dot_pos]=sr[dot_pos+1];
                sr[dot_pos+1]=ch;
            }
            if(sr[dot_pos]!='#')
                goto_no=item_search(sr);
            int xx,yy,zz;
            if(isnonterminal(ch2))
            {
                for(yy=0; ch2!=nonTer[yy]; yy++);
                tableLR1[i][number_of_terminal+1+yy]=goto_no;
            }
            else if(isterminal(ch2))
            {
                for(xx=0; ch2!=ter[xx]; xx++);
                t_no=xx;
                tableLR1[i][t_no]=goto_no;

            }
            else if(ch2==',')
            {
                reduced_rule=100;

                char sub_sr[100];
                int sub;
                for( sub=0; sr[sub]!='.'; sub++)
                {
                    sub_sr[sub]=sr[sub];

                }
                sub_sr[sub]='\0';

                for(zz=0; zz<=n; zz++)
                {

                    if(!strcmp(grammar[zz],sub_sr))
                    {
                        if(zz==0)
                            reduced_rule=no_of_states;
                        else
                            reduced_rule=zz;
                    }

                }
                for(zz=0; sr[zz]!=','; zz++);
                for(zz; sr[zz]!='\0'; zz++)
                {
                    if(isterminal(sr[zz]))
                    {
                        for(xx=0; sr[zz]!=ter[xx]; xx++);
                        t_no=xx;
                        tableLR1[i][t_no]=0-reduced_rule;
                    }
                    else if(sr[zz]=='$')
                    {

                        tableLR1[i][number_of_terminal]=0-reduced_rule;
                    }
                }
            }

        }
    }
}
void print_table()
{
    int i,j,k,l;
    for(i=0; i<number_of_terminal; i++)
    {
        printf("    %c      ",ter[i]);
    }
    printf("    $      ");
    for(i=0; i<number_of_nonterminal; i++)
    {
        printf("    %c      ",nonTer[i]);
    }
    printf("\n");
    for(i=0; i<no_of_states; i++)
    {
        for(j=0; j<(number_of_nonterminal+number_of_terminal+1); j++)
        {
            printf("%5d      ",tableLR1[i][j]);
        }
        printf("%\n");
    }
}
void stack_initialize()
{
    printf("0 is pushed\n");
    my_stack.push(0);

}
void augment_expression()
{
    int length_exp=strlen(expression);
    expression[length_exp]='$';
    expression[length_exp+1]='\0';
}
bool isState(char ch)
{
    int i,j,k;
    k=ch-48;
    if(ch>=0 && ch<=no_of_states)
    {
        return true;
    }
    else
        return false;

}

void string_parse()
{
    int i=0,j,k,current_exp_index=0,red_rule,state,action,length_rule;
    char stack_top,current_exp_ch,collumn;
    augment_expression();
    stack_initialize();
    while(1)
    {
        stack_top=my_stack.top();
        if(state==1 && current_exp_ch=='$')
        {
            printf("accepted\n");
            break;
        }
        if(isState(stack_top) )
        {
            if(stack_top==48)
                state=stack_top-48;
            else
                state=stack_top;
            current_exp_ch=expression[current_exp_index];
            collumn=Terminal_no(current_exp_ch);
            action=tableLR1[state][collumn];
            if(action==0)
            {
                printf("Rejected\n");
                break;
            }
            if(action>0)
            {
                printf("Shift %d\n",action);

                my_stack.push(current_exp_ch);
                my_stack.push(action);

                current_exp_index++;
            }
            else if(action<0)
            {
                red_rule=0-action;
                int isEpsilon_rule;
                isEpsilon_rule=epsilon(grammar[red_rule][0]);
                length_rule=strlen(grammar[red_rule])-2;
                if(red_rule<no_of_states)
                    printf("Reduce %d   .. %s \n",red_rule,grammar[red_rule]);
                    if(grammar[red_rule][2]!='#'){
                for(k=0; k<(2*length_rule); k++)
                {
                    my_stack.pop();
                }
                    }
                my_stack.push(grammar[red_rule][0]);
            }
        }
        else if(isnonterminal(stack_top))
        {
            my_stack.pop();
            int state_belo=my_stack.top();
            collumn=nonTerminal_no(stack_top)+number_of_terminal+1;
            my_stack.push(expression[ current_exp_index]);
            if(tableLR1[state_belo][collumn]>0)
            {
                my_stack.push(tableLR1[state_belo][collumn]);
            }
            else
            {
                printf("rejected");
                break;
            }
        }
    }
}

void match_new_items()
{
    int i,j,k;
    for(i=0; i<no_of_states; i++)
    {
        for(j=0; j<no_of_items[i]; j++)
        {
            for(k=0; k<strlen(items[i][j]); k++)
            {
                if(items[i][j][k]!=',')
                    new_items[i][j][k]=items[i][j][k];
                else
                {
                    new_items[i][j][k]='\0';
                    break;
                }
            }
        }
    }
}

void print2()
{
    int i, j;
    printf("\nNumber of states = %d.\n", no_of_states);
    for(i = 0; i < no_of_states; i++)
    {
        printf("\n\nItems in State %d...\n\n", i);

        for(j = 0; j < no_of_items[i]; j++)
            printf("%s\n", new_items[i][j]);
    }
}

int item_search_lalr(char sr[100],int x)
{
    int i,j,k,l,m,n;
    for(i=0; i<no_of_states; i++)
    {
        if(i!=x)
        {
            if(!strcmp(new_items[i][0],sr))
                return i;
        }
    }

    return 0;
}

int match_state_2[100];
int number_of_matched_state=0;
int isalready_matched(int p)
{
    int i,q;
    for(i=0; i<number_of_matched_state; i++)
    {
        if(match_state_2[i]==p)
            return 1;
    }
    return 0;
}

void initialize_LALR_table()
{
    int i,j;
    for(i=0; i<no_of_states; i++)
    {
        for(j=0; j<(number_of_nonterminal+number_of_terminal+1); j++)
        {
            tableLALR[i][j]=tableLR1[i][j];
        }
    }
}

void re_initialize_LR1_table()
{
    int i,j;
    for(i=0; i<no_of_states; i++)
    {
        for(j=0; j<(number_of_nonterminal+number_of_terminal+1); j++)
        {
            tableLR1[i][j]=tableLALR[i][j];
        }
    }
}
void table_for_LALR(int first_state, int second_state)
{
    int i,j,k,l,m;
    for(i=0; i<no_of_states; i++)
    {
        for(j=0; j<(number_of_nonterminal+number_of_terminal+1); j++)
        {
            if(tableLALR[i][j]==second_state)
            {
                tableLALR[i][j]=first_state;
            }
        }
        if(i==second_state)
        {

            for(j=0; j<(number_of_nonterminal+number_of_terminal+1); j++)
            {

                if(tableLALR[i][j]!=0)
                {
                    tableLALR[first_state][j]=tableLALR[second_state][j];
                    tableLALR[second_state][j]=0;
                }
            }

        }
    }

}

void print_table_LALR()
{
    int i,j,k,l,pos_of_ep=0;
    for(i=0; i<number_of_terminal; i++)
    {
        if(ter[i]=='#')
        {
            pos_of_ep=i;
        }
        else
        printf("    %c      ",ter[i]);
    }
    printf("    $      ");
    for(i=0; i<number_of_nonterminal; i++)
    {
        printf("    %c      ",nonTer[i]);
    }
    printf("\n");
    for(i=0; i<no_of_states; i++)
    {
        for(j=0; j<(number_of_nonterminal+number_of_terminal+1); j++)
        {
            if(j!=pos_of_ep)
            printf("%5d      ",tableLALR[i][j]);
        }
        printf("%\n");
    }
}


int main()
{
    int i,j,k,l,m,a,b;
    char str[100];

    printf("Number of production\n");
    scanf("%d",&n);
    for(i=1; i<=n; i++)
    {
        scanf("%s",grammar[i]);
    }
    strcpy(grammar[0], "U=");
    str[0] = grammar[1][0];
    str[1] = '\0';
    strcat(grammar[0], str);
    n++; //number of terminal
    make_terminal_nonterminal();
    char stdd[100];

    //compute first set
    printf("\nFirst set\n");
    for(i=0; i<strlen(nonTer); i++)
    {
        printf("%c: ",nonTer[i]);
        FIRSTcalc(Result,nonTer[i],stdd);
        int z=0;
        for(z=0; z<strlen(Result); z++)
        {
            firstset[i][z]=Result[z];
            printf("%c ",Result[z]);
        }
        printf("\n");
    }

    //compute follow set
    printf("\nFollow set\n");
    for(i=0; i<strlen(nonTer); i++)
    {

        FOLLOWcalc(nonTer[i]);
        int z=0;
        for(z=0; z<strlen(Result); z++)
        {
            followset[i][z]=Result[z];
        }
    }
    //print follow set
    int rule_no;
    for(i=0; i<strlen(nonTer); i++)
    {
        printf("%c :",nonTer[i]);
        for(j=0; j<strlen(followset[i]); j++)
        {
            printf("%c ",followset[i][j]);
        }
        printf("\n");
    }
    initialize_items();
    compute_closure_goto();
    print();
    table_initaialize();
    entry_table();
    match_new_items();
    int x2=0;
    for(i=0; i<no_of_states; i++)
    {
        x2=0;
        if(!isalready_matched(i))
            x2=item_search_lalr(new_items[i][0],i);
        if(x2!=0)
        {
            match_state_2[number_of_matched_state]=i;
            number_of_matched_state++;
            match_state_2[number_of_matched_state]=x2;
            number_of_matched_state++;
        }
    }
    initialize_LALR_table();
    for(i=0; i<number_of_matched_state; i=i+2)
    {
        printf("%d and %d will merged\n",match_state_2[i],match_state_2[i+1]);
        table_for_LALR(match_state_2[i],match_state_2[i+1]);
    }
    printf("LALR Table:\n");
    print_table_LALR();
    printf("Input expression\n");
    scanf("%s",&expression);
    string_parse();
}
