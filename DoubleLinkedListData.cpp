#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

struct Data{
	char Code[16], Name[16], Type[16], Unit[16];
	int Qty, Purchase, Sell; 
	Data *Back, *Next;
} *Head, *Tail, *Help, *New, *Del;

void NewData(){
	New = (struct Data*)malloc(sizeof(struct Data));
	printf("Input the data:\n");
	printf("Code			: "); scanf("%s", New->Code);				fflush(stdin);
	printf("Name			: "); scanf("%[^\n]", New->Name);			fflush(stdin);	
	printf("Type			: "); scanf("%s", New->Type);				fflush(stdin);
	printf("Qty			: "); scanf("%d", &New->Qty);					fflush(stdin);
	printf("Unit			: "); scanf("%s", New->Unit);				fflush(stdin);
	printf("Purchase		: "); scanf("%d", &New->Purchase);			fflush(stdin);
	printf("Sell			: "); scanf("%d", &New->Sell);				fflush(stdin);
}

void Message(char msg[]){
	printf("%s\n", msg);
	printf("Press any key to return to the main menu.\n");
	getch();
}

FILE *fData;
char NameFileData[]="DoubleLinkedListData.dbc";
int SizeFileData, SizeData=sizeof(Data), SizeRecData=SizeData-2*sizeof(Data *), TotalData;

int main(){
	
	char opt;
	int p;
	
	Head=Tail=NULL;
	
	fData=fopen(NameFileData,"rb");
	if(fData){
		fseek(fData, 0, SEEK_END);
		SizeFileData=ftell(fData);
		fseek(fData, 0, SEEK_SET);
		int n = SizeFileData/SizeRecData;
		for(int i = 0; i < n; i++){
			New = (struct Data*)malloc(sizeof(struct Data));
			fread(New, SizeRecData, 1, fData);
			New->Next=NULL;
			New->Back=NULL;
			if(Head){
				New->Back=Tail;
				Tail->Next=New;
				Tail=New;
			} else Tail=Head=New;
		}
	}
	
	do{
		system("cls");
		for(Help=Head; Help!=NULL; Help=Help->Next){
			printf("%-7s %-15s %-10s %3i %-3s %15i %15i\n", Help->Code, Help->Name, Help->Type, Help->Qty, Help->Unit, Help->Purchase, Help->Sell);
		}
		
		printf("\n\nMain Menu - Item Data (Double Linked List) :\n");
		printf("0. Initialize\n");
		printf("1. Insert a data in front\n");
		printf("2. Insert a data at the end\n");
		printf("3. Insert a data in certain position\n");
		printf("4. Delete the first data\n");
		printf("5. Delete the last data\n");
		printf("6. Delete a data in certain position\n");
		printf("Your Option [Esc to Exit] : ");
		opt=getch();
		printf("%c\n", opt);
		
		switch(opt){
			
			case '0':
				if(!Head){
					NewData();
					New->Next=NULL;
					New->Back=NULL;
					Head=Tail=New;					
				} else Message("Cannot create a new list because the first list already exists.");
				break;
			
			case '1':
				if(Head){
					NewData();
					New->Next=Head;
					Head->Back=NULL;
					Head=New;
				} else Message("Cannot add a new list because the list does not exist yet.");
				break;
			
			case '2':
				if(Head){
					NewData();
					New->Back=Tail;
					Tail->Next=New;
					Tail=New;
					Tail->Next=NULL;
				} else Message("Cannot add a new list because the list does not exist yet.");
				break;
				
			case '3':
				if(Head){
					NewData();
					printf("Enter the insertion position: "); scanf ("%d", &p);
					Help=Head;
					for(int i = 1; i < p; i++) Help=Help->Next;
					New->Next=Help->Next;
					Help->Next=New;
					New->Back=Help;
					Help=New->Next;
					Help->Back=New;
				} else Message("Cannot add a new list because the list does not exist yet.");
				break;
				
			case '4':
				if(Head){
					Del=Head;
					Head=Head->Next;
					if(Head) Head->Back=NULL;
					free(Del);
				} else Message("Cannot delete data because no list exists yet.");
				break;
			
			case '5':
				if(Head){
					Del=Tail;
					Tail=Tail->Back;
					if(Tail) Tail->Next=NULL;
					else Head=NULL;
					free(Del);
				} else Message("Cannot delete data because no list exists yet.");
				break;
				
			case '6':
				if(Head){
					Del=Head;
					printf("Enter the deletion position: "); scanf ("%d", &p);
					Help=Head;
					for(int i = 1; i < p; i++) Help=Help->Next;
					Del=Help->Next;
					Help->Next=Del->Next;
					Help=Del->Next;
					Help->Back=Del->Back;
					free(Del);
				} else Message("Cannot delete data because no list exists yet.");
				break;
			
		}
		
		
		
	}while(opt!=27);
	
//	fData=fopen(NameFileData, "wb");
	
	while(Head){
		Del=Head;
		Head=Head->Next;
//		if (fData) fwrite(Del, SizeRecData, 1, fData);
		free(Del);
	}
	
	if (fData) fclose(fData);
	
	return 0;
}
