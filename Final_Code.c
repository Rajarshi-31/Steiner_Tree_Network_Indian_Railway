#include <stdio.h>
#include <stdlib.h>
#define N 113471
#define M 4448

/*Graph Definations*/
struct Graph
{
    struct Node* head[N];
};
struct Node
{
    int dest, weight,flag;
    struct Node* next;
};
/*File loading*/
struct Edge {
    int src, dest, weight;
};
struct Array{
    int seq;
    char station[5];
    int value;
};
/* Component definations*/
struct Component{
    int weight;
    int active;
    struct Vertex* vertex;
};
struct Vertex{
    int node;
    struct Vertex* next;
};
/*Graph function Definations*/
struct Graph* createGraph(struct Edge edges[], int n)
{
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    for (int i = 0; i < N; i++) {
        graph->head[i] = NULL;
    }
    for (int i = 0; i < n; i++)
    {
        int src = edges[i].src;
        int dest = edges[i].dest;
        int weight = edges[i].weight;
        int flag=1;
        struct Node* ptr=graph->head[src];
            while(ptr!=NULL){
                    if(ptr->dest==dest){
                            flag=0;
                }
                ptr=ptr->next;
        }
        if(flag){
            struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
            newNode->dest = dest;
            newNode->weight = weight;
            newNode->flag=1;
            newNode->next = graph->head[src];
            graph->head[src] = newNode;
            struct Node* newNode1 = (struct Node*)malloc(sizeof(struct Node));
            newNode1->dest = src;
            newNode1->weight = weight;
            newNode1->flag=1;
            newNode1->next = graph->head[dest];
            graph->head[dest] = newNode1;
        }
    }
    return graph;
}
void printGraph(struct Graph* graph,struct Array arr[])
{
    for (int i = 0; i < N; i++)
    {
        struct Node* ptr = graph->head[i];
        while (ptr != NULL)
        {
            printf("%s[%d] -> %s[%d] (%d)\t", arr[i].station,arr[i].value, arr[ptr->dest].station, arr[ptr->dest].value,ptr->weight);
            ptr = ptr->next;
        }

        printf("\n");
    }
}
void check(struct Graph* graph,struct Array arr[], int n){
    struct Node*ptr=graph->head[n];
     while (ptr != NULL)
        {
            printf("%s[%d] -> %s[%d] (%d)\t", arr[n].station,arr[n].value, arr[ptr->dest].station, arr[ptr->dest].value,ptr->weight);
            ptr = ptr->next;
        }
}
/* Component function definations*/
struct Component initialization(struct Component component,int i,int root){
        component.weight=0;
        component.active=1;
        if(i==root) component.active--;
        struct Vertex* newnode = (struct Vertex*)malloc(sizeof(struct Vertex));
        newnode->node=i;
        newnode->next=NULL;
        component.vertex=newnode;
        return component;
}

void print(struct Component component[]){
    for(int i=0;i<N;i++){
        printf("%d:\n",i);
        printf("weight: %d\n",component[i].weight);
        printf("active: %d\n",component[i].active);
        struct Vertex *ptr=component[i].vertex;
        while ((ptr->next) != NULL)
        {
            printf("%d-->",ptr->node);
            ptr = ptr->next;
        }
        printf("%d",ptr->node);
        printf("\n");
    }
}
int main(void)
{
    FILE *file1;
	file1=fopen("F:\\NIT Internship\\Project\\Station_Info.txt","r");
	if(file1==NULL){
        printf("Error opening file.\n");
        return 1;
	}
	struct Array station[M];
	int read=0;
	int records=0;
	int j=0;
	do{
        read=fscanf(file1,"%[^,],%d,%d\n",station[records].station,&station[records].seq,&station[records].value);
        j++;
        if(read==3) records++;
        if(read!=3 && !feof(file1)){
            printf("File format incorrect\n");
            return 1;
        }
        if(ferror(file1)){
            printf("Error reading file.\n");
            return 1;
        }
	}while(!feof(file1));
    fclose(file1);
    printf("\n%d records read.\n\n", records);

	FILE *file2;
	file2=fopen("F:\\NIT Internship\\Project\\Routes.txt","r");
	if(file2==NULL){
        printf("Error opening file.\n");
        return 1;
	}
	read=0;
    records=0;
	j=0;
	struct Edge travel[N];
	do{
        read=fscanf(file2,"%d,%d,%d\n",&travel[records].src,&travel[records].dest,&travel[records].weight);
        j++;
        if(read==3) records++;
        if(read!=3 && !feof(file2)){
            printf("File format incorrect\n");
            return 1;
        }
        if(ferror(file2)){
            printf("Error reading file.\n");
            return 1;
        }
	}while(!feof(file2));
    fclose(file2);
    printf("\n%d records read.\n\n", records);

    int n = sizeof(travel)/sizeof(travel[0]);
    struct Graph *graph = createGraph(travel, n);
    printGraph(graph,station);
    //printf("check\n");
    //check(graph,arr,2);
    //printf("\n");

    struct Component component[N];
    int root=1;
    int d[N];
    for(int i=0;i<N;i++){
            component[i]=initialization(component[i],i,root);
            d[i]=0;
    }

    //print(component);
    printf("\n\n");
    int e1_min,e1,e2_min,e2,e;
    int s1,s2,sum,C2,temp,node2;
    int count=N-1;
    while(count!=0){
        e1_min=INT_MAX;
        e2_min=INT_MAX;
        for (int i = 0; i < N; i++)
        {
            /*e1 min*/
            struct Vertex *ptr1=component[i].vertex;
            int comp1=ptr1->node;
            struct Node* ptr = graph->head[i];
            while (ptr != NULL)
            {
                int dest=ptr->dest;
                int weight=ptr->weight;
                int comp2=(component[dest].vertex)->node;
                if(comp1!=comp2 &&ptr->flag!=0){
                    if((component[comp1].active+component[comp2].active)!=0){
                        e1=(weight-d[i]-d[dest])/(component[comp1].active+component[comp2].active);
                    }
                    if(e1<e1_min){
                        e1_min=e1;
                        s1=i;
                        s2=dest;
                    }
                }
                ptr = ptr->next;
            }
            /*e2 min*/
            if(component[i].active==1){
                sum=0;
                struct Vertex *ptr4=component[i].vertex;
                while(ptr4!=NULL)
                {
                    sum+=station[ptr4->node].value;
                    ptr4=ptr4->next;
                }
                e2=sum-component[i].weight;
                if(e2<e2_min){
                    e2_min=e2;
                    C2=i;
                }
            }
        }
        (e1>e2)?(e=e2):(e=e1);

        for(int i=0;i<N;i++){
                component[i].weight+=e*component[i].active;
        }
        temp=component[root].vertex->node;
        struct Vertex* ptr5=component[temp].vertex;
        while(ptr5!=NULL){
            d[ptr5->node]+=e*component[temp].active;
            ptr5=ptr5->next;
        }
        if(e==e2){
            component[C2].active=0;
        }
        else{
            struct Node* ptr8 = graph->head[s1];
            while (ptr8 != NULL)
            {
                node2=ptr8->dest;
                if(node2==s2)   ptr8->flag=0;
                ptr8 = ptr8->next;
            }
            struct Node* ptr9 = graph->head[s2];
            while (ptr9 != NULL)
            {
                node2=ptr9->dest;
                if(node2==s1)   ptr9->flag=0;
                ptr9 = ptr9->next;
            }
            struct Vertex *ptr3=component[s1].vertex;
            s1=ptr3->node;
            ptr3=component[s1].vertex;

            temp=s2;
            struct Vertex *ptr4=component[s2].vertex;
            s2=ptr4->node;
            ptr4=component[s2].vertex;
            component[s2].active=2;
            if(temp!=s2)    (component[temp].vertex)->node=s1;
            while((ptr3->next)!=NULL){
                ptr3=ptr3->next;
            }
            ptr3->next=ptr4;
            struct Vertex* newnode2 = (struct Vertex*)malloc(sizeof(struct Vertex));
            newnode2->node=s1;
            newnode2->next=NULL;
            component[s2].vertex=newnode2;
            while(ptr4!=NULL){
                (component[ptr4->node].vertex)->node=s1;
                ptr4=ptr4->next;
            }
            component[s1].weight+=component[s2].weight;
            int flag=0;
            ptr3=component[s1].vertex;
            while(ptr3!=NULL){
                if(ptr3->node==root){
                    flag=1;
                }
                ptr3=ptr3->next;
            }
            if(flag)    component[s1].active=0;
            else    component[s1].active=1;
        }
        count=0;
        for(int i=0;i<N;i++){
            if(component[i].active==1){
                count+=component[i].active;
            }
        //if(count==1)    print(component);
        }
    }
    //print(component);
    //for(int i=0;i<N;i++){
    //    printf("%d: %d\n",i,d[i]);
    //}
    return 0;
}
