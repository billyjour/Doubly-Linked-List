#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

struct Data{
	char Code[16], Name[16], Type[16], Unit[16];
	int Qty, Purchase, Sell; 
	Data *Back, *Next;
} *Head, *Tail, *Help, *New, *Del, **ArrData;

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
	char FindCode[16], TempCode[16];
	int i, l, r, m;
	TotalData=0;
	Head=Tail=NULL;
	
	fData=fopen(NameFileData,"rb");
	if(fData){
		fseek(fData, 0, SEEK_END);
		SizeFileData=ftell(fData);
		fseek(fData, 0, SEEK_SET);
		int n = SizeFileData/SizeRecData;
		TotalData=n;
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
		
		free(ArrData);
		ArrData=(struct Data**)malloc(TotalData*sizeof(struct Data*));
		i=0;
		for(Help=Head; Help!=NULL; Help=Help->Next){
			ArrData[i++]=Help;
			printf("%-7s %-15s %-10s %3i %-3s %15i %15i\n",
			Help->Code, Help->Name, Help->Type, Help->Qty, Help->Unit, Help->Purchase, Help->Sell);
		}
		
		printf("\n\nMain Menu - Item Data (Double Linked List) :\n");
		printf("A. Add Data\n");
		printf("D. Delete Data\n");
		printf("S. Search Data\n");
		printf("M. Modify Data\n");
		printf("Your Option [Esc to Exit] : ");
		opt=getch();
		printf("%c\n", opt);
		
		switch(opt){
			
			case 'A': case 'a':
				NewData();
				if(!Head){
					New->Next=NULL;
					New->Back=NULL;
					Head=Tail=New;
				} else if(strcmp(New->Code, Head->Code) < 0){
					New->Back=NULL;
					New->Next=Head;
					Head=New;
				} else if(strcmp(New->Code, Tail->Code) > 0){
					New->Back=Tail;
					New->Next=NULL;
					Tail->Next=New;
					Tail=New;
				} else{
					Help=Head;
					while (strcmp(New->Code, Help->Next->Code) > 0) Help=Help->Next;
					New->Next=Help->Next;
					Help->Next=New;
					New->Back=Help;
					Help=New->Next;
					Help->Back=New;
				}
				TotalData++;
				break;
				
			case 'D': case'd':
				if(Head){
					printf("Enter the item code to be deleted: "); scanf("%[^\n]", FindCode);		fflush(stdin);
					strupr(FindCode);
					if(strcmp(FindCode, Head->Code) == 0){
						Del=Head;
						Head=Head->Next;
						if(Head) Head->Back=NULL;
						free(Del);
						TotalData--;
					} else if(strcmp(FindCode, Tail->Code) == 0){
						Del=Tail;
						Tail=Tail->Back;
						if(Tail) Tail->Next=NULL;
						else Head=NULL;
						free(Del);
						TotalData--;
					} else{
						Del=NULL;
						Help=Head;
						while(Help->Next && !Del){
							if(strcmp(FindCode, Help->Next->Code) == 0){
								Del=Help->Next;	
							} else Help=Help->Next;
						}
						if(!Del){
							Message("Item code not found.");
						} else {
							Help=Del->Back;
							Help->Next=Del->Next;
							Help=Del->Next;
							Help->Back=Del->Back;
							free(Del);
							TotalData--;	
						}
						
					}
				} else Message("Cannot delete item because no list exists yet.");
				break;
				
			case 'S': case 's':
				printf("Enter the item code to search: "); scanf("%[^\n]", FindCode);			fflush(stdin);
				strupr(FindCode);
				l=0;
				r=TotalData-1;
				Help=NULL;
				//using binary search
				while(l<=r && !Help){
					m=(l+r)/2;
					strcpy(TempCode, ArrData[m]->Code);
					strupr(TempCode);
					if(strcmp(FindCode, TempCode) == 0){
						Help=ArrData[m];
					} else if(strcmp(FindCode, TempCode) < 0){
						r=m-1;
					} else{
						l=m+1;
					}
				}if (!Help) Message ("The item code was not found.");
					else {
						puts("Item found:");
						printf("Code			: %s\n", Help->Code);				
						printf("Name			: %s\n", Help->Name);			
						printf("Type			: %s\n", Help->Type);			
						printf("Qty			: %d\n", Help->Qty);				
						printf("Unit			: %s\n", Help->Unit);			
						printf("Purchase		: %d\n", Help->Purchase);		
						printf("Sell			: %d\n", Help->Sell);		
						getch();	
					}
				break;
					
			case 'M': case'm':
				printf("Enter the item code to modify: "); scanf("%[^\n]", FindCode);			fflush(stdin);
				strupr(FindCode);
				New=NULL;
				for(Help=Head; Help!=NULL; Help=Help->Next){
					strcpy(TempCode, Help->Code);
					strupr(TempCode);
					if(strcmp(FindCode, TempCode) == 0) New=Help;
				} if(!New) Message ("The item code was not found.");
					else{
						puts("Item found:");
						printf("Code			: %s\n", New->Code);				
						printf("Name			: %s\n", New->Name);			
						printf("Type			: %s\n", New->Type);			
						printf("Qty			: %d\n", New->Qty);				
						printf("Unit			: %s\n", New->Unit);			
						printf("Purchase		: %d\n", New->Purchase);		
						printf("Sell			: %d\n", New->Sell);
						printf("\n\nPlease modify the following data:\n");
						printf("Name			: "); scanf("%[^\n]", New->Name);			fflush(stdin);	
						printf("Type			: "); scanf("%s", New->Type);				fflush(stdin);
						printf("Unit			: "); scanf("%s", New->Unit);				fflush(stdin);
						printf("Purchase		: "); scanf("%d", &New->Purchase);			fflush(stdin);
						printf("Sell			: "); scanf("%d", &New->Sell);				fflush(stdin);
					}
					break;
				
				
				
				
				
			
/*			case '0':
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
*/		
	
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
