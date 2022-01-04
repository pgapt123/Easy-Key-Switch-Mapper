#include<bits/stdc++.h>
#define For(i,a,b) for(int i=a,_=b;i<=_;++i)
#define Fdn(i,b,a) for(int i=b,_=a;i>=_;--i)
#define sqr(x) ((x)*(x))
#define bin(x) (1<<((x)-1))
#define ful(x) ((1<<(x))-1)

//#define DEBUG

#ifdef DEBUG
	#define dprt(...) printf(__VA_ARGS__)
#else
	#define dprt(...)
#endif

using namespace std;
typedef long long LL;

mt19937 mt_rand(time(0));

struct Grp{
	int id;
	string name;

	void print(FILE *fout){
		fprintf(fout, "\t{ \"id\" : %d, \"name\" : \"%s\" }", id, name.c_str());
	}
};

struct Art{
	int id, idx, gp;
	int keyno;
	string name;
	int vel;
	unsigned int col;
	
	void print(FILE *fout){
		fprintf(fout, 
				"\
				{\n\
					\"id\" : %d, \"name\" : \"%s\", \"index\" : %d,  \"groupId\" : %d, \"color\" : \"%x\", \"duration\" : 120,\n\
					\"events\" : \n\
					[\n\
						{ \"b1\":144, \"b2\":%d, \"b3\":%d, \"b4\":0, \"allowTranspose\":0, \"allowTransposeMidiCh\":1, \"triggerAt\":4, \"chaseMode\":3 }\n\
					],\n\
					\"transforms\" :[]\n\
				}",
				id, name.c_str(), idx, gp, col,
				keyno, vel
		);
	}
};

vector<Grp> gp;
vector<Art> art;


void suspend(){
	puts("Some thing went wrong :( Press ANY key to exit...\n");
	system("pause");
	exit(0);
}


void usgerr(){
	puts(
		"USAGE ERROR!\n"
		"Usage: ezksmp [args] [text file]\n"
		"-h for help"
	);
	suspend();
}


void synerr(FILE *fin, char *buf, string info="Unkonwn Error"){
	int lineno = 1;

	int curpos = ftell(fin);
	//dprt("Bytes: %d\n",bytes);
	fseek(fin, 0, SEEK_SET);
	while(ftell(fin) < curpos){
		char ch = fgetc(fin);
		//dprt("|%c %ld\n", ch, ftell(fin));
		if(ch=='\n') lineno++;
	}

	printf("SYNTAX ERROR in line %d:\n    ", lineno);
	cout << info << endl;
	printf("    Check around \"%s\" !\n", buf);
	puts("-h for help");
	suspend();
} 


void indent(int cnt){
	For(i,1,cnt) printf("\t");
}


bool str_is_num(char *s){
	if(!isdigit(s[0]) && s[0]!='-') return 0;
	For(i,1,strlen(s)-1){
		if(!isdigit(s[i])) return 0;
	}
	return 1;
}


void parse_file(FILE *fin){
	int cnt=-1, lineno=1;
	int keyno=-1;
	int vel;
	unsigned int col;
	static char buf[1024], artname[1024], gpname[1024];
	
	int art_idx = -1;		// index in one group
	int art_id = 0;			// global art id (unrepeatable)
	
	dprt("bp1\n");
	
	while(fscanf(fin, "%s", buf) != EOF){
		
		//dprt("cnt: %d buf: %s\n",cnt,buf);
		
		// parse current string
		dprt("bp1.5\n");
		int buflen = strlen(buf); 
		
		if(cnt==-1){		// A NEW LINE
			dprt("bp2\n");
			lineno++;
			if(buf[0] == '-'){		// a new group
				sscanf(buf+1, "%s", gpname);
				int gp_id = gp.size()+1;
				gp.push_back(
					(Grp){gp_id, gpname}
				);
				//art_idx = -1;
				
				continue; 	// the outer while
				
			}else{					// a new articulation in last group, initialize
				//dprt("bp3\n");
				art_id++;
				art_idx++;
				
				if(gp.size()==0){		// no group created, create a default group
					gp.push_back(
						(Grp){1, "Articulation"}
					);
				}
				
				sprintf(artname, "Art %d", art_id);	// default name 
				
				vel = 100;
				//dprt(">>>\n");
				col = 0xff000000 | (mt_rand()%(1<<24));		// random color, 0xffRRGGBB
				//dprt("<<<\n");
				
				cnt = 0;
			}
			
		}
		
		if(cnt==0){			// key
			dprt("key > %s", buf);
			if(isdigit(buf[0])){
				if(!str_is_num(buf)) synerr(fin, buf, "Key expression wrong");
				sscanf(buf, "%d", &keyno);
				if(keyno<0 || keyno>127) synerr(fin, buf, "Key No. must in [0..127]");
				
			}else if('A'<=buf[0] && buf[0]<='G'){
				switch(buf[0]){
					case 'C': keyno=0; break;
					case 'D': keyno=2; break;
					case 'E': keyno=4; break;
					case 'F': keyno=5; break;
					case 'G': keyno=7; break;
					case 'A': keyno=9; break;
					case 'B': keyno=11; break;
				}
				int numbg = 1;
				if(buf[1]=='#'){
					keyno++;
					numbg = 2;
				}else if(buf[1]=='b'){
					keyno--;
					numbg = 2;
				}
				if(!str_is_num(buf+numbg)) synerr(fin, buf, "Key expression wrong");
				int octno;
				sscanf(buf+numbg, "%d", &octno);
				keyno = keyno + 12*(octno+2);
				if(keyno<0 || keyno>127) synerr(fin, buf, "Key No. must in [0..127]");
				
			}else if(buf[0]=='=' && buflen==1){
				if(keyno==-1) synerr(fin, buf, "Stepping needs a starting point!");
				// keyno is equal to last keyno
			
			}else if(buf[0]=='+'){
				if(keyno==-1) synerr(fin, buf, "Stepping needs a starting point!");
				For(i,0,buflen-1){
					if(buf[i]!='+') synerr(fin, buf, "Use only '+' for stepping!");
					keyno++;
				}
				if(keyno<0 || keyno>127) synerr(fin, buf, "Key No. must in [0..127]");
				
			}else{
				synerr(fin, buf, "Key expression wrong");
			}
			
		}else if(cnt==1){ 	// art name
			sprintf(artname,"%s",buf);
			
		}else{				// others
			char op = buf[0];
			switch(op){
				case 'v':		//velocity
					sscanf(buf+1, "%d", &vel);
					if(vel<0 || vel>127) synerr(fin, buf, "Velocity value must in [0..127]");
					break;
				case 'c':		//color
					sscanf(buf+1, "%x", &col);
					if(col>0xffffff) synerr(fin, buf, "Color value wrong! Use 'RRGGBB' !");
					col = (int)0xff000000 | col;
					break;
				default:
					synerr(fin, buf, "Unknown operation! (Tip: DO NOT leave blanks at the end of line OR in names!)");
			}
			
		}
		
		// is end of line?
		char bk = 0;
		if(fscanf(fin, "%c", &bk)==EOF || bk=='\n'){
			cnt = -1;
			// save articulation in the vectors
			art.push_back(
				(Art){
					art_id, art_idx, (int)gp.size(),
					keyno,
					artname,
					vel,
					col
				}
			);
			
			
		}else{
			cnt++;
			
		}
		
	}
	
}

			
void write_output(FILE *fout, string artmap_name){
	// Head
	fprintf(fout, "{\"ArticulationMaps\" :[{\n");
	fprintf(fout, "\"name\" : \"%s\",\n", artmap_name.c_str());
	
	// Groups
	fprintf(fout, "\"groups\" :[\n");
	For(i, 0, gp.size()-2){
		gp[i].print(fout); fprintf(fout, ",\n");
	}
	gp.back().print(fout); fprintf(fout, "\n],\n");
	
	// Arts
	fprintf(fout, "\"articulations\" :[\n");
	For(i, 0, art.size()-2){
		art[i].print(fout); fprintf(fout, ",\n");
	}
	art.back().print(fout); fprintf(fout, "\n]\n");
	
	// End
	fprintf(fout, "}]}\n");
}
	

int main(int argc, char **argv){
	if(argc <= 1){
		usgerr();
	}
	if(argv[1][0] == '-'){
		switch(argv[1][1]){
			case 'v':
				puts("V1.2");
				puts("This is a little tool for easily creating Cakewalk keyswitch artmap!");
				puts("It makes *.artmap files with indents, for you to easily read and modify them!");
				puts("It was developed in C++ so it runs ultra fast!");
				puts("Developed by APT 2021.1.16~18");
				puts("- ALL RIGHTS RESERVED -");
				return 0;			
			case 'h':
				system("\"Easy Key Switch Mapper User Manual.html\"");
				return 0;
			default:
				usgerr();
		}
	}
	
	FILE *in = fopen(argv[argc-1], "r");
	//string artmap_name = strtok(argv[argc-1], ".");
	
	
	dprt("Ready to parse file!\n");
	
	char artmap_name_buf[1024];
	fgets(artmap_name_buf, 1024, in);
	string artmap_name = strtok(artmap_name_buf, "\n");
	
	parse_file(in);
	
	string outname = artmap_name + ".artmap";
	FILE *out = fopen(outname.c_str(), "w"); 
	
	write_output(out, artmap_name);
	
	printf("Successfully created Cakewalk artmap file \"%s\" !\n", outname.c_str());
	printf("Press ANY key to exit...\n\n");
	system("pause");
	
	return 0;
}
	
	
	
			
	
	
