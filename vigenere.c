#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define ARR_SIZE 36

int decrypt(char *k, char *c, char *result);
int encrypt(char *k, char *p, char *result);
void showUsage(void);

int main(int argc, char * argv[]) {
    

    	if (argc != 6 && argc != 8){ 
        	printf("enter 6 or 8 arguments only, you have: %d . see usage below for help\n", argc);
		int i;
		for(i = 0; i < argc; i++)
		{
			printf("argv[%d] is %s\n", i, argv[i]);
		}
		showUsage();
        	return 1; 
    	}
    	else if (argc == 6){
		//printf ("\n%s\n%s\n%s\n%s\n%s\n", argv[1],argv[2],argv[3],argv[4],argv[5]);
		if(strcmp(argv[1],"-e") == 0 && strcmp(argv[2],"-k") == 0 && strcmp(argv[4], "-i") == 0)
		{
			encrypt(argv[3], argv[5], ""); 
		}
		else if(strcmp(argv[1],"-d") == 0 && strcmp(argv[2],"-k") == 0 && strcmp(argv[4], "-i") == 0)
		{
			decrypt(argv[3], argv[5], "");
		}
		else
		{
			printf("You have entered improper flags.  See usage and try again\n");
			showUsage();
			return 1;
		}
	}
	else if (argc == 8){
		//printf ("\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n", argv[1], argv[2],argv[3],argv[4],argv[5],argv[6], argv[7]);
		if(strcmp(argv[1],"-E") == 0 && strcmp(argv[2],"-k") == 0 && strcmp(argv[4],"-i") == 0 && strcmp(argv[6],"-o") == 0)
		{
			encrypt(argv[3], argv[5], argv[7]);
		}
		else if(strcmp(argv[1],"-D") == 0 && strcmp(argv[2],"-k") == 0 && strcmp(argv[4],"-i") == 0 && strcmp(argv[6],"-o") == 0)
		{
			decrypt(argv[3], argv[5], argv[7]);
		}
		else
		{
			printf("You have entered improper flags.  See usage and try again\n");
			showUsage();
			return 1;
		}
	}
	else
	{
		printf("Sorry, invalid input. Please try again and see usage below for help\n");
		showUsage();
	}	

    	
	
    	//if (argv[1] == "-e" && argv[2] == "-k" && argv[3] ==   
    	return 0;
}

int decrypt(char *k, char *c, char *result) {

    	char alphabet[ARR_SIZE] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    	char tabula_recta[ARR_SIZE][ARR_SIZE] = {0};  //Tabula Recta

	int i,h,j = 0;
	for( i=0; i<ARR_SIZE; i++ ) 
	{
    		for( h=0; h<ARR_SIZE; h++ ) 
		{
        		tabula_recta[i][h] = alphabet[ (j+h) % ARR_SIZE ];
    		}
    		j++;
	}

	if(strcmp(result, "") == 0)//if is is non-file based
	{
		int i;
		for(i = 0; i < strlen(k); i++)
		{
			char ch = k[i];
			if(!((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')))
    			{
       		 		printf("ONLY UPPERCASE ALPHANUMERIC CHARACTERS ALLOWED IN KEY. PLEASE TRY AGAIN\n");
				return -1;
    			}
		}

		for(i = 0; i < strlen(c); i++)
		{
			char ch = c[i];
			if(!((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')))
    			{
       		 		printf("ONLY UPPERCASE ALPHANUMERIC CHARACTERS ALLOWED IN CIPHERTEXT, PLEASE TRY AGAIN\n");
				return -1;
    			}
		}

		char * keystream = malloc(sizeof(char) * strlen(c));

		if(strlen(k) < strlen(c))
		{
			int i;
			for(i = 0; i < strlen(k); i++)
			{
				keystream[i] = k[i];
			}
			int j;
			for(j = strlen(k); j < strlen(c); j++)
			{
				keystream[j] = k[j % strlen(k)];
			}	
		}
		else//if the key is the same length or greater
		{
			int i;
			for(i =0; i < strlen(c); i++)
			{
				keystream[i] = k[i];
			}
		}

		char * plaintext = malloc(sizeof(char) *strlen(c));
		char row;
		char col;
		for(i = 0; i < strlen(c); i++)
		{
			
			col = keystream[i];
			int r;
			int keyChar;

			if(col >= 65 && col <= 90)
			{
				keyChar = col - 65;
			}
			else if(col >= 48 && col <= 57)
			{
				keyChar = col - 22;
			} 
			else
			{
				printf("error happened in col calculation\n");
				return -1;
			}
			
			for(r = 0; r < ARR_SIZE; r++)
			{
				if(tabula_recta[r][keyChar] == c[i])
				{
					if(r < 26)
					{
						r += 65;
					}
					else
					{
						r += 22;
					}
				}
			}
			row = r - 1;
			plaintext[i] = row;
			
			
		}
		printf("%s\n", plaintext); 
		
		free(keystream);
		free(plaintext);
		
	}
	else//if is is file based
	{
		char key[512];
		char cipher[512];
		FILE * keyFile;
		FILE * cipherFile;
		FILE * plainFile;

		if((keyFile = fopen(k, "r")) == NULL)
		{
			perror("could not open key file");
			return -1;
		}

		if((cipherFile = fopen(c, "r")) == NULL)
		{
			perror("could not open cipher file");
			return -1;
		}
		
		
		if((plainFile = fopen(result, "w+")) == NULL)
		{
			perror("could not open plaintext result file");
			return -1;
		}

		char c;
		c = fgetc(keyFile);
		int i = 0; 
    		while (c != EOF) 
    		{ 
    			key[i] = c; 
        		c = fgetc(keyFile);
			i++; 
    		} 
    		fclose(keyFile);
		key[i - 1] = '\0';

		int j = 0;
		c = fgetc(cipherFile); 
    		while (c != EOF) 
    		{ 
			cipher[j] = c;
        		c = fgetc(cipherFile); 
			j++;
    		} 
    		fclose(cipherFile);
		cipher[j - 1] = '\0';

		//printf("key is %s and cipher is %s\n", key, cipher);

		
		
		for(i = 0; i < strlen(key); i++)
		{
			char ch = key[i];
			if(!((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')))
    			{
       		 		printf("ONLY UPPERCASE ALPHANUMERIC CHARACTERS ALLOWED IN KEY. PLEASE TRY AGAIN\n");
				return -1;
    			}
		}

		for(i = 0; i < strlen(cipher); i++)
		{
			char ch = cipher[i];
			if(!((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')))
    			{
       		 		printf("ONLY UPPERCASE ALPHANUMERIC CHARACTERS ALLOWED IN CIPHERTEXT, PLEASE TRY AGAIN\n");
				return -1;
    			}
		}

		char * keystream = malloc(sizeof(char) * strlen(cipher));

		if(strlen(key) < strlen(cipher))
		{
			int i;
			for(i = 0; i < strlen(key); i++)
			{
				keystream[i] = key[i];
			}
			int j;
			for(j = strlen(key); j < strlen(cipher); j++)
			{
				keystream[j] = key[j % strlen(key)];
			}	
		}
		else//if the key is the same length or greater
		{
			int i;
			for(i =0; i < strlen(cipher); i++)
			{
				keystream[i] = key[i];
			}
		}

		char * plaintext = malloc(sizeof(char) *strlen(cipher));
		char row;
		char col;
		for(i = 0; i < strlen(cipher); i++)
		{
			
			col = keystream[i];
			int r;
			int keyChar;

			if(col >= 65 && col <= 90)
			{
				keyChar = col - 65;
			}
			else if(col >= 48 && col <= 57)
			{
				keyChar = col - 22;
			} 
			else
			{
				printf("error happened in col calculation\n");
				return -1;
			}
			
			for(r = 0; r < ARR_SIZE; r++)
			{
				if(tabula_recta[r][keyChar] == cipher[i])
				{
					if(r < 26)
					{
						r += 65;
					}
					else
					{
						r += 22;
					}
				}
			}
			row = r - 1;
			plaintext[i] = row;
			
			
		}
		//printf("%s\n", plaintext);
		fputs(plaintext,plainFile); 
		fclose(plainFile);
		free(keystream);
		free(plaintext);

	}
	return 1;
}

int encrypt(char *k, char *p, char *result) {

	char alphabet[ARR_SIZE] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    	char tabula_recta[ARR_SIZE][ARR_SIZE] = {0};  //Tabula Recta

	int i,h,j = 0;
	for( i=0; i<ARR_SIZE; i++ ) 
	{
    		for( h=0; h<ARR_SIZE; h++ ) 
		{
        		tabula_recta[i][h] = alphabet[ (j+h) % ARR_SIZE ];
    		}
    		j++;
	}

	if(strcmp(result, "") == 0)//if is is non-file based
	{
		int i;
		for(i = 0; i < strlen(k); i++)
		{
			char ch = k[i];
			if(!((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')))
    			{
       		 		printf("ONLY UPPERCASE ALPHANUMERIC CHARACTERS ALLOWED IN KEY. PLEASE TRY AGAIN\n");
				return -1;
    			}
		}

		for(i = 0; i < strlen(p); i++)
		{
			char ch = p[i];
			if(!((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')))
    			{
       		 		printf("ONLY UPPERCASE ALPHANUMERIC CHARACTERS ALLOWED IN PLAINTEXT, PLEASE TRY AGAIN\n");
				return -1;
    			}
		}

		char * keystream = malloc(sizeof(char) * strlen(p));

		if(strlen(k) < strlen(p))
		{
			int i;
			for(i = 0; i < strlen(k); i++)
			{
				keystream[i] = k[i];
			}
			int j;
			for(j = strlen(k); j < strlen(p); j++)
			{
				keystream[j] = k[j % strlen(k)];
			}	
		}
		else//if the key is the same length or greater
		{
			int i;
			for(i =0; i < strlen(p); i++)
			{
				keystream[i] = k[i];
			}
		}

		char * ciphertext = malloc(sizeof(char) *strlen(p));
		char row;
		char col;
		for(i = 0; i < strlen(p); i++)
		{
			row = p[i];
			col = keystream[i];
			int r;
			int c;
			if(row >= 65 && row <= 90)
			{
				r = row - 65;
			}
			else if(row >= 48 && row <= 57)
			{
				r = row - 22;
			}
			else
			{
				printf("error happened in row calculation\n");
			}

			//column now

			if(col >= 65 && col <= 90)
			{
				c = col - 65;
			}
			else if(col >= 48 && col <= 57)
			{
				c = col - 22;
			} 
			else
			{
				printf("error happened in col calculation\n");
			}

			ciphertext[i] = tabula_recta[r][c];
			
		}
		//ciphertext[i] = '\0';
		printf("%s\n", ciphertext); 
		
		free(keystream);
		free(ciphertext);
	}
	else//if is is file based
	{
		char key[512];
		char plain[512];
		FILE * keyFile;
		FILE * cipherFile;
		FILE * plainFile;

		if((keyFile = fopen(k, "r")) == NULL)
		{
			perror("could not open key file");
			return -1;
		}

		
		if((plainFile = fopen(p, "r")) == NULL)
		{
			perror("could not open plaintext file");
			return -1;
		}

		if((cipherFile = fopen(result, "w+")) == NULL)
		{
			perror("could not open cipher result file");
			return -1;
		}

		char c;
		c = fgetc(keyFile);
		int i = 0; 
    		while (c != EOF) 
    		{ 
    			key[i] = c; 
        		c = fgetc(keyFile);
			i++; 
    		} 
    		fclose(keyFile);
		key[i - 1] = '\0';

		int j = 0;
		c = fgetc(plainFile); 
    		while (c != EOF) 
    		{ 
			plain[j] = c;
        		c = fgetc(plainFile); 
			j++;
    		} 
    		fclose(plainFile);
		plain[j - 1] = '\0';

		for(i = 0; i < strlen(key); i++)
		{
			char ch = key[i];
			if(!((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')))
    			{
       		 		printf("ONLY UPPERCASE ALPHANUMERIC CHARACTERS ALLOWED IN KEY. PLEASE TRY AGAIN\n");
				return -1;
    			}
		}

		for(i = 0; i < strlen(plain); i++)
		{
			char ch = plain[i];
			if(!((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')))
    			{
       		 		printf("ONLY UPPERCASE ALPHANUMERIC CHARACTERS ALLOWED IN PLAINTEXT, PLEASE TRY AGAIN\n");
				return -1;
    			}
		}

		char * keystream = malloc(sizeof(char) * strlen(plain));

		if(strlen(key) < strlen(plain))
		{
			int i;
			for(i = 0; i < strlen(key); i++)
			{
				keystream[i] = key[i];
			}
			int j;
			for(j = strlen(key); j < strlen(plain); j++)
			{
				keystream[j] = key[j % strlen(key)];
			}	
		}
		else//if the key is the same length or greater
		{
			int i;
			for(i =0; i < strlen(plain); i++)
			{
				keystream[i] = key[i];
			}
		}

		char * ciphertext = malloc(sizeof(char) *strlen(plain));
		char row;
		char col;
		for(i = 0; i < strlen(plain); i++)
		{
			row = plain[i];
			col = keystream[i];
			int r;
			int kschar;
			if(row >= 65 && row <= 90)
			{
				r = row - 65;
			}
			else if(row >= 48 && row <= 57)
			{
				r = row - 22;
			}
			else
			{
				printf("error happened in row calculation\n");
			}

			//column now

			if(col >= 65 && col <= 90)
			{
				kschar = col - 65;
			}
			else if(col >= 48 && col <= 57)
			{
				kschar = col - 22;
			} 
			else
			{
				printf("error happened in col calculation\n");
				return -1;
			}

			ciphertext[i] = tabula_recta[r][kschar];
			
		}
		//printf("%s\n", ciphertext); 
		fputs(ciphertext,cipherFile); 
		fclose(cipherFile);
		free(keystream);
		free(ciphertext);
			
	}
	
	return 1;
}

void showUsage() {

    printf("\nVigenère is a simple substitution cipher program\n");

    printf("\nUse Case: Argument-Based\
\n\nE(k,p): vigenere -e -k KEYTEXT(all caps) -i PLAINTEXT(all caps also)\
\nD(k,c): vigenere -d -k KEYTEXT(all caps) -i CIPHERTEXT(all caps also)\n\n");

    printf("\nUse Case: File-Based\
\n\nE(k,p): vigenere -E -k 'kpath' -i 'ppath' -o 'cpath'\
\nD(k,c): vigenere -D -k 'kpath' -i 'cpath' -o 'ppath'\n TEXT IN FILES MUST BE ALL CAPS\n\n");

}
