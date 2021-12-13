#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <cctype>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
using namespace std;
using std::cout; using std::cin;
using std::endl; using std::string;
using std::vector; using std::istringstream;
using std::stringstream;
//Metodos globales
void ParticionPrimaria(int,char,string,char,string);
void PaticionExtendida(int,char,string,char,string);
void ParticionLogica(int,char,string,char,string);
bool ExisteParticion(string,string);
void MontarParticion(string,string);

int FindLogic(string , string );
int FindPrimariaYExtendida(string , string );
void MontarParticion(string , string );
int FindLetra(string , string );
void ImprimirParticinesMontadas();

int FindNumero(string ,string );
void DesmontarParticion(string);
void FormatEXT2(int,int,string);
void FormatEXT3(int,int,string);
void ComandoMKFS(string,string,int);
int VerificarLogin(string,string,string,string);
int FindGrupo(string);
int VerificarParametros(string,string,string);
char ObtenerFit(string,string);
void login(string,string,string);
void logout();


//mmadf
void MKGRP(string);
int ObtenerIDG();
void SaveJournaling(char* ,int ,int ,char *,char *);
void AddUserstxt(string );
int FindBit(FILE *, char , char );
void RMGRP(string);
void REMOVERGRUPO(string);

void MKUSR(string , string , string );
bool buscarUsuario(string );
int getID_usr();
void RMUSR(string );
void eliminarUsuario(string );
void cambiarPermisosRecursivo(FILE* , int , int perisos);
void CHMOD(string , string ,bool );
int buscarCarpetaArchivo(FILE *, char* );
int byteInodoBloque(FILE *,int , char );
vector<string> Mkfsids;

//Variables globales
//TODO:ARREGLAR PATH 
string path="/home/eduardo/Escritorio/ArchivosVacas/Proyecto_1/Proyecto_1";
bool SesionActiva=false;

//ESTRUCTURAS GENERALES
struct ActiveSession{
    int IDU;
    int IDG;
    int PosicionInicialSB;
    int PosicionInicialJournal;
    int FS;
    char fit;
    string direccion;
};
struct Particion
{
    char part_status;
    char part_type = 'P'; // P de primaria como default
    char part_fit = 'W'; // W de peor ajuste por default 
    int part_start;
    int part_size;
    char part_name[16];
};
struct MBR
{
    int mbr_tamano;
    time_t mbr_fecha_creacion;
    int mbr_disk_signature;
    char disk_fit;
    Particion mbr_partition[4];
};
struct EBR{
    char part_status; 
    char part_fit; 
    int part_start; 
    int part_size; 
    int part_next; 
    char part_name[16]; 
};
struct DiscoD{ 
	string path="";
	int size;
	string unit="M"; /// k=kb m=mb megabytes como default
    char fit='F';//Primer ajuste como default.
}
Disk1;
struct nodoParticionMontada {
    string path;
    string name;
    char letra;
    int numero;
};
struct SuperBloque{
    int s_filesystem_type; //Guarda el numero que identifica al sistea de archivos utilizado
    int s_inodes_count; //Guarda el numero total de inodos
    int s_blocks_count; //Guarda el numero total de bloques
    int s_free_blocks_count; //Contiene el numero de bloques libres
    int s_free_inodes_count; //Contiene el numero inodos libres
    time_t s_mtime; //Ultima fecha en el que el sistema fue montado
    time_t s_umtime; //Ultima fecha en que el sistema fue desmontado
    int s_mnt_count; //Indica cuantas veces se ha montado el sistema
    int s_magic; //Valor que identifica al sistema de archivos 0xEF53
    int s_inode_size; //Tamano del Inodo
    int s_block_size; //Tamano del bloque
    int s_first_ino; //Primer Inodo libre
    int s_first_blo; //Primero bloque libre
    int s_bm_inode_start; //Guardara el inicio del bitmap de inodos
    int s_bm_block_start; //Guardara el inicio del bitmap de bloques
    int s_inode_start; //Guarada el inicio de la tabla de inodos
    int s_block_start; //Guardara el inicio de la tabla de bloques
};
struct Tabla_Inodos{
    int i_uid; //UID del usuario propiertario del archivo/carpeta
    int i_gid; //GID del grupo al que pertenece el archivo/carpeta
    int i_size; //Tamano del archivo en bytes
    time_t i_atime; //Ultima fecha en que se leyo el Inodo sin modificarlo
    time_t i_ctime; //Fecha en que se creo el el Inodo
    time_t i_mtime; //Ultima fecha en la que se modifco
    int i_block[15]; //Array de bloques
    char i_type; //Indica si es archivo o carpeta
    int i_perm; //Guarada los permisos del archivo/carpeta
    
};
struct Bloque_Archivos{
    char b_content[64];
};
struct Content{
    char b_name[12];//Nombre carpeta/archivo
    int b_inodo;//Apuntador hacia un Inodo asociado al archivo o carpeta
};
struct Bloque_Carpetas{
    Content b_content[4];//Array con el contenido de la carpeta
};
struct Journaling{
    char Journal_tipo_operacion[10];
    int Journal_tipo;//Archivo/Carpeta
    char Journal_nombre[100];
    char Journal_contenido[100];
    time_t Journal_fecha;
    int Journal_propietario;
    int Journal_permisos;
};
struct BloqueDeApuntadores{
    int b_pointers[16];//Array con los apuntadores hacia bloques
};
void CrearDisco(DiscoD op){
cout<<"\033[92m************************CREANDO DISCO*************************\033[0m\n"<<endl;
    string s = path + op.path;//url raiz + url de la entrada
    char sc[s.size() + 1];
    strcpy(sc, s.c_str());
    FILE *file=NULL;
    file=fopen(sc,"r"); //r= read = sera verdadero si el disco ya Exist
    if(file!=NULL){
        cout<<"\033[91mYa Exist el disco\033[0m"<<endl;
        return;//error
    }
    //Tamaño para distintas unidades que se piden
    int tam;

    if(op.unit=="K"){ //kb
      tam =  op.size* 1024;
    }
    else if(op.unit == "M"){ //mb
        tam =  op.size* 1024 * 1024;
    }

    file=fopen(sc,"wb");
    fwrite("\0",1,1,file);
    /*se pone el puntero en el tamaño deseado del archivo y esto automaticamente 
	hace que el archivo tenga ese size*/
    fseek(file,tam,SEEK_SET); // objeto file, size de cuanto se quiere mover, al inicio
    fwrite("\0",1,1,file);
    //////ESCRITURA DEL MBR/////
    //Modificacion del struct MBR
    MBR mbr;
    mbr.mbr_tamano = tam;
    mbr.mbr_disk_signature = rand()%10000;
    mbr.mbr_fecha_creacion = time(0);
    mbr.disk_fit = op.fit;
    for(int i = 0; i < 4; i++){
        mbr.mbr_partition[i].part_status = '0';
        mbr.mbr_partition[i].part_size = 0;
        mbr.mbr_partition[i].part_fit = 'W';
        mbr.mbr_partition[i].part_start = -1;
        strcpy(mbr.mbr_partition[i].part_name,"");
    }
    
    cout<<"\033[92mDisco nuevo :: \033[0m"<<op.path<<"\n \033[92mFecha de creacion: \033[0m"<<asctime(gmtime(&mbr.mbr_fecha_creacion))<<endl;
    cout<<"\033[92mSignature: \033[0m"<<mbr.mbr_disk_signature <<endl;
    cout<<"\033[92mTamaño: \033[0m"<<mbr.mbr_tamano <<" \033[92mBytes\033[0m"<<endl;
    cout<<"\033[92mFit:\033[0m " <<mbr.disk_fit <<endl;
    cout<<"\n\033[92m-------------DISCO CREADO--------------------\n\033[0m"<<endl;
    ///escritura del mbr
    fseek(file,0,SEEK_SET);
    fwrite(&mbr,sizeof(MBR),1,file);
    fclose(file);
}
//FUNCIONES Y METODOS A UTILIZAR 
ActiveSession SesionActual;
//Funcion para transformar la entrada a letra mayuscula y asi poder hacer una aplicacion case-insensitive
string CastearMayuscula(char cad1[]){
    int i;
    for ( i = 0; i < strlen(cad1); i++ )  {
    cad1[i] = toupper( cad1[i] );
}
    return cad1;
}
/**
 * Funcion que retorna un vector spliteado por el delimitador que se le ingrese a la cadena deseada para separar.
 * */
vector<string> Split(string cadena,string delimitador){
    size_t pos_start = 0, pos_end, delim_len = delimitador.length();
    string llave;
    vector<string> res;

    while ((pos_end = cadena.find (delimitador, pos_start)) != string::npos) {
        llave = cadena.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(llave);
    }

    res.push_back(cadena.substr (pos_start));
    return res;
}
/**
 * Funcion que recibe un string y splitea la cadena por espacios retorna un vector con lo spliteado
 * */
vector<string> SplitSpace(string text){
    vector<string> lineSplit{};
    string word;
    stringstream sstream(text);
    while (getline(sstream,word,' ')){
        lineSplit.push_back(word);
    }
    return lineSplit;
}
void mkcarpetas(string entrada){
    vector<string> aux2;
    aux2 = Split(entrada,"/");//Se crea un vector que esta spliteado por el simbolo / para poder crear las carpetas deseadas por si no existen.
    string variable;//String que almacenara las carpetas a crear o creadas
    for(size_t j=1; j<(aux2.size()-1); j++){//Ciclo que se recorre desde 1 hasta el tamanio del path menos 1 ya que la ultima posicion contiene el nombre deldisco  a crear, se empieza de 1 ya que la posicion 0 es posicion vacia que esta antes de el primer simboo /
        variable =variable+aux2[j]+"/";
        char sc[variable.size() + 1];
        strcpy(sc,variable.c_str());//Casteamos el string a char ya que la funcion mkdir recibe una variable de tipo char* con la direccion del directorio a crear
        mkdir(sc, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); //creamos la carpeta recursivamene                       
    }
}
void borrardisco(string ruta){//Metodo que recibe como parametro un path a eliminar.
    if(remove(ruta.c_str())==0) // Eliminamos el archivo
    {
        cout<<"\033[92mEl archivo fue eliminado satisfactoriamente\033[0m\n"<<endl;
    }
    else{
        cout<<"\033[91mError: No se pudo eliminar el archivo\033[0m\n"<<endl;
    }
}
void EjecutarComando(char comando[200]){
    char temporal[200];
    strcpy(temporal,comando);
    if(comando[0]!='#'){//Validacion por si viene un comentario antes de una instruccion o comando
        for(int i=0;i<=200;i++){
            if(comando[i]=='#'){//validacion por si viene un comentario al final de la linea
                memcpy(comando,comando,i);//Copiamos la linea del comando pero antes del llegar al comentario
            }
        }
        string comandoCasteado;
        comandoCasteado = CastearMayuscula(comando);//Casteamos todo a mayusculas para trabajarlo internamente porque pueden venir mayusculas y minusculas
        int aa = strncmp(comando,"PAUSE",5);
        if(aa==0){
            int pause;
            cout<<"\033[91m******       Script pausado!       ******\n"<<"******Presione Enter para continuar******\033[0m\n";
            //pausamos
            pause = cin.get();
        }
        vector<string>lineSplit = SplitSpace(comandoCasteado);//Spliteamos por espacios
        vector<string>lineSplitSinCasteo = SplitSpace(temporal);
            if(lineSplit[0]=="MKDISK"){
                int sizeerror,fiterror,uniterror =0;
                bool unit,fit = false;//Variables booleanas que se estableceran por defecto si no se declaran en el comando si son false se pondran en automatico por defecto segun enunciado
                vector<string> auxiliar;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    if(auxiliar[0] == "-PATH"){//Si el comando es el -path entonces entrara a esta condicional
                        mkcarpetas(auxiliar[1]);//Creamos las carpetas con el path ingresado.
                        Disk1.path = auxiliar[1];//Asignamos el path al disco
                    }else if(auxiliar[0]=="-FIT"){//Asignamos el fit al disco
                        if(auxiliar[1]=="BF"){
                            Disk1.fit= 'B';
                            fit = true;
                        }else if(auxiliar[1]=="FF"){
                            Disk1.fit= 'F';
                            fit = true;
                        }else if(auxiliar[1]=="WF"){
                            Disk1.fit= 'W';
                            fit = true;
                        }else{
                            fiterror = 1;
                            cout<<" \033[91mError: Esta tratando de ingresar un ajuste no permitido\033[0m"<<endl;
                        }    
                    }
                    else if(auxiliar[0]=="-UNIT"){//Asignamos la unidad al disco
                        if(auxiliar[1]=="M" || auxiliar[1]=="K"){
                            Disk1.unit = auxiliar[1];
                            unit = true;
                        }else{
                            uniterror = 1;
                            cout<<"\033[91mError: Esta tratando de ingresar una unidad no permitida para crear disco\033[0m"<<endl;
                        }
                        
                    }
                    else if(auxiliar[0] == "-SIZE"){//Asignamos el tamanio al disco
                        if(stof(auxiliar[1])<=0){
                            sizeerror = 1;
                            cout<<"\033[91mError: El tamaño del disco no puede ser negativo o igual a cero\033[0m"<<endl;
                        }else{
                            Disk1.size = stof(auxiliar[1]);
                        }                        
                    }
                }
                if(fit==false){//Asignacion default
                    Disk1.fit = 'F';
                }
                if(unit==false){//Asignacion default
                    Disk1.unit ="M";
                }
                if(sizeerror==1 || uniterror==1 || fiterror==1){
                    cout<<"\033[91mError: No se pudo crear el disco!!\033[0m"<<endl;
                }else{
                    CrearDisco(Disk1);//Creamos disco
                }                
            }else if(lineSplit[0]=="RMDISK"){
                string dir;
                vector<string> aux;
                aux = Split(lineSplit[1],"~:~");
                if(aux[0] == "-PATH"){
                    dir = path+aux[1];
                    borrardisco(dir); 
                }           
            }else if(lineSplit[0]=="FDISK"){
                int sizeerror,fiterror,uniterror,typeerror =0;
                int sizepart;
                char unitpart;
                string rutapart;
                char fitpart;
                string namepart; 
                char typepart;
                bool deffit,deftype,defunit=false;
                vector<string> auxiliar;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    if(auxiliar[0] == "-SIZE"){//Si el comando es el -path entonces entrara a esta condicional
                        if(stof(auxiliar[1])<=0){
                            sizeerror = 1;
                            cout<<"\033[91mError: Esta tratando de ingresar una unidad no permitida para crear particion\033[0m"<<endl;
                        }else{
                            sizepart = stof(auxiliar[1]);
                        }
                    }else if(auxiliar[0]=="-UNIT"){
                        if(auxiliar[1]=="M"){
                            unitpart = 'M';
                            defunit = true;
                        }else if(auxiliar[1]=="K"){
                            unitpart = 'K';
                            defunit = true;
                        }else if(auxiliar[1]=="B"){
                            unitpart = 'B';
                            defunit = true;
                        }else{
                            uniterror = 1;
                            cout<<"\033[91mError: Esta tratando de ingresar una unidad no permitida para crear una particion\033[0m"<<endl;
                        }    
                    }else if(auxiliar[0]=="-PATH"){
                        rutapart = auxiliar[1];    
                    }else if(auxiliar[0]=="-TYPE"){
                        if(auxiliar[1]=="P"){
                            typepart = 'P';
                            deftype = true;
                        }else if(auxiliar[1]=="E"){
                            typepart = 'E';
                            deftype = true;
                        }else if(auxiliar[1]=="L"){
                            typepart = 'L';
                            deftype = true;
                        }else{
                            typeerror = 1;
                            cout<<"\033[91mError: Esta tratando de ingresar un tipo incompatible con las que se pueden crear particiones utiliza P,E o L\033[0m"<<endl;
                        } 
                    }else if(auxiliar[0]=="-FIT"){//Asignamos el fit a la particion
                        if(auxiliar[1]=="BF"){
                            fitpart = 'B';
                            deffit = true;
                        }else if(auxiliar[1]=="WF"){
                            fitpart = 'W';
                            deffit = true;
                        }else if(auxiliar[1]=="FF"){
                            fitpart = 'F';
                            deffit = true;
                        }else{
                            fiterror = 1;
                            cout<<"\033[91mError: Esta tratando de ingresar un ajuste no permitido para la particion\033[0m"<<endl;
                        }    
                    }else if(auxiliar[0]=="-NAME"){
                        namepart = auxiliar[1];                       
                    }
                }
                if(!defunit){//Establecemos valor default de las unidades si no viene en el comando
                    unitpart = 'K';
                }
                if(!deffit){//Establecemos valor default de el fit si no viene en el comando.
                    fitpart = 'W';
                }
                if(!deftype || typepart=='P'){//Creamos particion primaria si no viene establecido en el comando el tipo de particion
                    //Creamos la particion primaria
                    cout<<"\033[93m-----Datos Partición PRIMARIA a Crear-----\033[0m"<<endl;
                    cout<<"\033[93msize:\033[0m "<<sizepart<<endl;
                    cout<<"\033[93munit:\033[0m "<<unitpart<<endl;
                    cout<<"\033[93mruta:\033[0m "<<rutapart<<endl;
                    cout<<"\033[93mfit:\033[0m "<<fitpart<<endl;
                    cout<<"\033[93mname:\033[0m "<<namepart<<endl;
                    ParticionPrimaria(sizepart,unitpart,rutapart,fitpart,namepart);

                }else if(deftype){
                    if(typepart=='E'){
                    cout<<"\033[93m-----Datos Partición EXTENDIDA a Crear-----\033[0m"<<endl;
                    cout<<"\033[93msize:\033[0m "<<sizepart<<endl;
                    cout<<"\033[93munit:\033[0m "<<unitpart<<endl;
                    cout<<"\033[93mruta:\033[0m "<<rutapart<<endl;
                    cout<<"\033[93mfit:\033[0m "<<fitpart<<endl;
                    cout<<"\033[93mname:\033[0m "<<namepart<<endl;
                        PaticionExtendida(sizepart,unitpart,rutapart,fitpart,namepart);
                    }else if(typepart=='L'){
                    cout<<"\033[93m-----Datos Partición LOGICA a Crear-----\033[0m"<<endl;
                    cout<<"\033[93msize:\033[0m "<<sizepart<<endl;
                    cout<<"\033[93munit:\033[0m "<<unitpart<<endl;
                    cout<<"\033[93mruta:\033[0m "<<rutapart<<endl;
                    cout<<"\033[93mfit:\033[0m "<<fitpart<<endl;
                    cout<<"\033[93mname:\033[0m "<<namepart<<endl;
                        ParticionLogica(sizepart,unitpart,rutapart,fitpart,namepart);
                    }
                }  
            }else if(lineSplit[0] == "MOUNT"){
                string ruta, nombre;
                vector<string> auxiliar;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    if(auxiliar[0] == "-PATH"){//Si el comando es el -path entonces entrara a esta condicional
                        ruta = auxiliar[1];
                    }else if(auxiliar[0]=="-NAME"){
                        nombre = auxiliar[1];
                    }
                }
                MontarParticion(nombre,ruta);
            }else if(lineSplit[0] == "UNMOUNT"){
                string id;
                vector<string> auxiliar;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    if(auxiliar[0] == "-ID"){//Si el comando es el -path entonces entrara a esta condicional
                        id = auxiliar[1];
                    }
                }
                DesmontarParticion(id);
            }else if(lineSplit[0] == "MKFS"){
                string id;
                bool flag = true;
                string type = "FULL";
                int fs=2;
                vector<string> auxiliar;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    if(auxiliar[0] == "-ID"){
                        id = auxiliar[1];
                    }else if(auxiliar[0] == "-TYPE"){
                        if(auxiliar[1]=="FULL" || auxiliar[1] == "FAST"){
                            type = auxiliar[1];
                        }else{
                            flag = false;
                            cout<<"\033[91mError:El tipo ingresado es invalido\033[0m"<<endl;
                        }                        
                    }else if(auxiliar[0] == "-FS"){
                        auto arreglochar = auxiliar[1].c_str();
                        if((arreglochar[0]=='2')&&(arreglochar[1]=='F')&&(arreglochar[2]=='S')){
                            fs=2;
                        }else if((arreglochar[0]=='3')&&(arreglochar[1]=='F')&&(arreglochar[2]=='S')){
                            fs=3;
                        }else{
                            flag = false;
                            cout<<"\033[91mError:El fs ingresado es invalido\033[0m"<<endl;
                        }
                    }
                }
                if(flag){
                    ComandoMKFS(id,type,fs);
                }                
            }else if(lineSplit[0] == "LOGIN"){
                string id,usuario,contrasenia;
                bool flag = true;
                vector<string> auxiliar;
                vector<string> auxiliarSinCastear;
                if(lineSplit.size()!=4){
                    flag=false;
                }
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    auxiliarSinCastear = Split(lineSplitSinCasteo[i],"~:~");
                    if(auxiliar[0] == "-USR"){
                        usuario = auxiliarSinCastear[1];
                    }else if(auxiliar[0] == "-PWD"){
                        contrasenia = auxiliarSinCastear[1];
                    }else if(auxiliar[0] == "-ID"){
                        id = auxiliar[1];
                    }
                }
                if(flag){
                    login(usuario,contrasenia,id);
                }else{
                   cout<<"\033[91mError: Error en el comando \033[0m"<<endl; 
                }                
            }else if(lineSplit[0] == "LOGOUT"){
                logout();                          
            }else if(lineSplit[0] == "MKGRP"){
                vector<string> auxiliar;
                vector<string> auxiliarSinCastear;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    auxiliarSinCastear = Split(lineSplitSinCasteo[i],"~:~");
                    if(auxiliar[0] == "-NAME"){
                        MKGRP(auxiliarSinCastear[1]);
                    }
                }              
            }else if(lineSplit[0] == "RMGRP"){
                vector<string> auxiliar;
                vector<string> auxiliarSinCastear;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    auxiliarSinCastear = Split(lineSplitSinCasteo[i],"~:~");
                    if(auxiliar[0] == "-NAME"){
                        RMGRP(auxiliarSinCastear[1]);
                    }
                }              
            }else if(lineSplit[0] == "MKUSR"){
                vector<string> auxiliar;
                vector<string> auxiliarSinCastear;
                string usuario;
                string pass;
                string grupo;
                bool flag=true;
                if(lineSplit.size()!=4){
                    flag=false;
                }
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    auxiliarSinCastear = Split(lineSplitSinCasteo[i],"~:~");
                    if(auxiliar[0] == "-USR"){
                        usuario = auxiliarSinCastear[1];
                    }else if(auxiliar[0] == "-PWD"){
                        pass = auxiliarSinCastear[1];
                    }
                    else if(auxiliar[0] == "-GRP"){
                        grupo = auxiliarSinCastear[1];
                    }
                }
                if(flag){
                    MKUSR(usuario,pass,grupo);
                }
            }else if(lineSplit[0] == "RMUSR"){
                vector<string> auxiliar;
                vector<string> auxiliarSinCastear;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    auxiliarSinCastear = Split(lineSplitSinCasteo[i],"~:~");
                    if(auxiliar[0] == "-USR"){
                        RMUSR(auxiliarSinCastear[1]);
                    }
                }              
            }else if(lineSplit[0] == "CHMOD"){
                bool r =false;
                string path,ugo;
                vector<string> auxiliar;
                vector<string> auxiliarSinCastear,auxiliarr;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    auxiliarr = Split(lineSplit[i],"-");
                    auxiliarSinCastear = Split(lineSplitSinCasteo[i],"~:~");
                    if(auxiliar[0] == "-PATH"){
                        path = auxiliar[1];
                    }else if(auxiliar[0] == "-UGO"){
                       ugo = auxiliar[1];
                    }else if(auxiliarr[1] == "R"){
                        r = true;
                    }
                }
                CHMOD(path,ugo,r);              
            }
    }
}
void leerscript(string ruta){//Metodo que recibe como parameto la ruta del script con comandos a ejecutar
    string linea;
    char line[200];
    ifstream file(ruta.c_str());
    while (getline(file,linea)){
        if(!linea.empty()){
            strcpy(line,linea.c_str());
            cout<<"\033[91mSCRIPT:\033[0m " << linea<<endl;
            EjecutarComando(line);
        }
    }
    /*FILE *script;
    if((script = fopen(ruta.c_str(),"r"))){//Si se encuentra el archivo y se puede abrir con exito
        char line[200]="";//declaramos un arreglo de char para poder almacenar linea individualmente 
        memset(line,0,sizeof(line));
        while(fgets(line,sizeof line,script)){//Mientras se lea una linea con salto de linea se ejecutara el while  
            if(line[0]!='\n'){//Obtenemos el dato de cada linea
                cout <<"SCRIPT: "<< line << endl;//Imprimimos el comando
                //lineas.push_back(line);//Pusheamos la linea al arreglo
                EjecutarComando(line);//Ejecutamos el comando
            }
            memset(line,0,sizeof(line));
        }
        fclose(script);//Cerramos el archivo.
    }else{
        cout << "Error al abrir el SCRIPT" << endl;
    }    */
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
bool ExisteParticion(string ruta, string name){
    int extendida = -1;
    FILE *filiPart;
    if((filiPart = fopen(ruta.c_str(),"rb+"))){
        MBR mbr;
        fseek(filiPart,0,SEEK_SET);
        fread(&mbr, sizeof(MBR),1,filiPart);
        for(int i = 0; i < 4; i++){
            if(strcmp(mbr.mbr_partition[i].part_name,name.c_str()) == 0){
                fclose(filiPart);
                return true;
            }else if(mbr.mbr_partition[i].part_type == 'E'){
                extendida = i;
            }
        }
        if(extendida != -1){
            fseek(filiPart, mbr.mbr_partition[extendida].part_start,SEEK_SET);
            EBR ebr;
            while((fread(&ebr,sizeof(EBR),1,filiPart))!=0 && (ftell(filiPart) < (mbr.mbr_partition[extendida].part_size + mbr.mbr_partition[extendida].part_start))){
                if(strcmp(ebr.part_name,name.c_str()) == 0){
                    fclose(filiPart);
                    return true;
                }
                if(ebr.part_next == -1){
                    fclose(filiPart);
                    return false;
                }
            }
        }
    }
    fclose(filiPart);
    return false;
}
//FDISK MANEJO DE PARTICIONES
void ParticionPrimaria(int size,char unit,string ruta,char fit,string name){
    char fittmp = 0;
    char unittmp = 0;
    int sizetmp;
    char buffer = '1';
    if(fit!=0){//Verificamos si no viene "vacio" el fit.
        fittmp = fit;
    }else{
        fittmp = 'W';//Peor ajuste por default
    }
    //Validamos la unidad y le damos el tamanio a la particion dependiendo de la unidad ingresada.
    if(unit!=0){
        unittmp = unit;
        if(unittmp == 'B'){
            sizetmp = size;
        }else if(unittmp == 'K'){
            sizetmp = size *1024;
        }else if(unittmp == 'M'){
            sizetmp = size * 1024 * 1024;
        }else{
            cout<<"\033[91mError: No esta ingresando una unidad permitida para la creacion de la particion primaria!!\033[0m"<<endl;
        }
    }else{
        unittmp = 'K';
        sizetmp = size * 1024;
    }

    FILE *fileprimary;
    MBR mbr;
    string rutatmp = path + ruta; 
    if((fileprimary = fopen(rutatmp.c_str(),"rb+"))){//Abrimos la ruta del disco y lo abrimos con rb+ para actualizar el archivo binario permitiendo tanto lectura como escritura 
        bool espacioDisponible = false;//Varible para saber si hay espacio para agregar la particion
        int contadorParticion = 0;//Contador para ver cuantas particiones hay.
        fseek(fileprimary,0,SEEK_SET);//Sitúa el puntero de lectura/escritura de un archivo en la posición indicada, 0 para que se situe al inicio del archivo
        fread(&mbr,sizeof(MBR),1,fileprimary);//Leemos el archivo mbr que esta dentro del disco.
        //Verificamos en el mbr si hay particiones disponibles.
        for(int i=0; i < 4; i++){
            if(mbr.mbr_partition[i].part_start == -1 || (mbr.mbr_partition[i].part_status=='1') && (mbr.mbr_partition[i].part_size>=sizetmp) ){
                espacioDisponible = true;
                contadorParticion = i;
                break;
            }
        }
        if(espacioDisponible){//verificamos si el espacio si es suficiente o no
            int bytes=0;
            for(int i=0; i < 4; i++){
                if(mbr.mbr_partition[i].part_status!='1'){//verificamos que las particiones no esten en estado activo
                    bytes += mbr.mbr_partition[i].part_size;//Sumamos el espacio disponible en las particiones NO ACTIVAS
                }
            }
            cout<<"\033[96mEspacio Disponible: "<<mbr.mbr_tamano-bytes << " Bytes\033[0m" <<endl;
            cout<<"\033[96mEspacio Necesario para particion primaria: "<<sizetmp<<" Bytes\033[0m"<<endl;
            if((mbr.mbr_tamano-bytes)>=sizetmp){//Verificamos si el tamanio es suficiente para poder aniadir la particion
                if(!ExisteParticion(rutatmp,name)){
                    if(mbr.disk_fit=='F'){//First Fit
                        mbr.mbr_partition[contadorParticion].part_type = 'P';
                        mbr.mbr_partition[contadorParticion].part_fit = fittmp;
                        if(contadorParticion==0){
                            mbr.mbr_partition[contadorParticion].part_start = sizeof(mbr);
                        }else{
                            mbr.mbr_partition[contadorParticion].part_start=mbr.mbr_partition[contadorParticion-1].part_start+mbr.mbr_partition[contadorParticion-1].part_size;
                        }
                        mbr.mbr_partition[contadorParticion].part_size = sizetmp;
                        mbr.mbr_partition[contadorParticion].part_status = '0';
                        strcpy(mbr.mbr_partition[contadorParticion].part_name,name.c_str());
                        fseek(fileprimary,0,SEEK_SET);
                        fwrite(&mbr,sizeof(MBR),1,fileprimary);//Guardamos en el nuevo mbr
                        fseek(fileprimary,mbr.mbr_partition[contadorParticion].part_start,SEEK_SET);//Guardamos los bytes de la particion
                        for(int i=0; i < sizetmp; i++){
                            fwrite(&buffer,1,1,fileprimary);
                        }
                        cout<<"\n \033[96mParticion PRIMARIA creada con exito de tipo FIRST FIT!!!\033[0m"<<endl;
                    }else if(mbr.disk_fit=='B'){//Best Fit
                        int best = contadorParticion;
                        for(int i = 0; i < 4; i++){
                            if(mbr.mbr_partition[i].part_start == -1 || (mbr.mbr_partition[i].part_status == '1' && mbr.mbr_partition[i].part_size>=sizetmp)){
                                if(i != contadorParticion){
                                    if(mbr.mbr_partition[best].part_size > mbr.mbr_partition[i].part_size){
                                        best = i;
                                        break;
                                    }
                                }
                            }
                        }
                        mbr.mbr_partition[best].part_type='P';
                        mbr.mbr_partition[best].part_fit = fittmp;
                        //Incio de particion
                        if(best==0){//Primera posicion
                            mbr.mbr_partition[best].part_start = sizeof(mbr);
                        }else{
                            mbr.mbr_partition[best].part_start = mbr.mbr_partition[best-1].part_start + mbr.mbr_partition[best-1].part_size;//Iniciamos la particion en la primera posicion vacia.
                        }
                        mbr.mbr_partition[best].part_size = sizetmp;
                        mbr.mbr_partition[best].part_status = '0';//Estado Activado.
                        strcpy(mbr.mbr_partition[best].part_name,name.c_str());//Renombramos
                        //Guardamos en MBR new
                        fseek(fileprimary,0,SEEK_SET);
                        fwrite(&mbr,sizeof(MBR),1,fileprimary);//Guardamos en el nuevo mbr
                        fseek(fileprimary,mbr.mbr_partition[best].part_start,SEEK_SET);//Guardamos los bytes de la particion
                        for(int i=0; i < sizetmp; i++){
                            fwrite(&buffer,1,1,fileprimary);
                        }
                        cout<<"\n \033[96mParticion PRIMARIA creada con exito de tipo BEST FIT!!!\033[0m"<<endl;
                    }else if(mbr.disk_fit == 'W'){//WORST FIT
                        int  worst= contadorParticion;
                        for(int i = 0; i < 4; i++){
                            if(mbr.mbr_partition[i].part_start == -1 || (mbr.mbr_partition[i].part_status == '1' && mbr.mbr_partition[i].part_size>=sizetmp)){
                                if(i != contadorParticion){
                                    if(mbr.mbr_partition[worst].part_size < mbr.mbr_partition[i].part_size){
                                        worst = i;
                                        break;
                                    }
                                }
                            }
                        }
                        mbr.mbr_partition[worst].part_type = 'P';
                        mbr.mbr_partition[worst].part_fit = fittmp;
                        //Inicio de la particion
                        if(worst == 0){//Por si es la primera opcion
                            mbr.mbr_partition[worst].part_start = sizeof(mbr);
                        }else{
                            mbr.mbr_partition[worst].part_start = mbr.mbr_partition[worst-1].part_start + mbr.mbr_partition[worst-1].part_size;
                        }
                        mbr.mbr_partition[worst].part_size = sizetmp;
                        mbr.mbr_partition[worst].part_status = '0';//Activamos la particion
                        strcpy(mbr.mbr_partition[worst].part_name,name.c_str());
                        //Guardamos en MBR nes
                        fseek(fileprimary,0,SEEK_SET);
                        fwrite(&mbr,sizeof(MBR),1,fileprimary);//Guardamos en el nuevo mbr
                        fseek(fileprimary,mbr.mbr_partition[worst].part_start,SEEK_SET);//Guardamos los bytes de la particion
                        for(int i = 0; i < sizetmp; i++){
                            fwrite(&buffer,1,1,fileprimary);
                        }
                        cout <<"\n \033[96mParticion PRIMARIA creada con exito de tipo WORST FIT!!!\033[0m"<<endl;
                    }
                
                }else{
                    cout<<"\033[91mError: La particion ya Exist!!\033[0m"<<endl;
                }  
            }else{
                cout<<"\033[91mError: No hay tanto espacio disponible, intenta con una particion que no exceda el espacio libre\033[0m"<<endl;
            }
        }else{
            cout<<"\033[91mError: Ya se posee el numero maximo de particiones que son 4, elimina una para poder crear otra particion primaria!!\033[0m"<<endl;
        }
        fclose(fileprimary);//Cerramos disco.
    }else{
        cout<<"\033[91mError: No se pudo abrir el disco, es posible que no exista!!\033[0m"<<endl;
    }
}
void PaticionExtendida(int size,char unit,string ruta,char fit,string name){
    char fitpartExtend = 0;
    char unitpartExtend = 0;
    string rutacompletaExtend = path + ruta;
    int BytesExtend;
    char buffer = '1';
    if(fit != 0){
        fitpartExtend = fit;

    }else{
        fitpartExtend = 'W';
    }
    if(unit != 0){
        unitpartExtend = unit;
        if(unitpartExtend == 'B'){
            BytesExtend = size;
        }else if(unitpartExtend == 'K'){
            BytesExtend = size * 1024;
        }else if (unitpartExtend=='M'){
            BytesExtend = size*1024*1024;
        }
    }else{
        BytesExtend = size * 1024;
    }

    FILE *fileDisk;
    MBR mbrE;
    if((fileDisk = fopen(rutacompletaExtend.c_str(), "rb+"))){
        bool PartDisponible,ExisteExtendida = false;//Variables para verificar si hay particion disponible en disco y para verificar si hay una particion extendida
        int ContadorPart = 0;
        fseek(fileDisk,0,SEEK_SET);
        fread(&mbrE,sizeof(MBR),1,fileDisk);
        for(int i = 0; i < 4; i++){
            if (mbrE.mbr_partition[i].part_type == 'E'){
                ExisteExtendida = true;
                break;
            }
        }
        if(!ExisteExtendida){
            for(int i = 0; i < 4; i++){
                if(mbrE.mbr_partition[i].part_start == -1 || (mbrE.mbr_partition[i].part_status == '1' && mbrE.mbr_partition[i].part_size>=BytesExtend)){
                    PartDisponible = true;
                    ContadorPart = i;
                    break;
                }
            }
            if(PartDisponible){
                int espacioUsado = 0;
                for(int i = 0; i < 4; i++){
                    if(mbrE.mbr_partition[i].part_status!='1'){
                       espacioUsado += mbrE.mbr_partition[i].part_size;
                    }
                }
                cout << "\033[96mEspacio disponible para partición extendida: " << (mbrE.mbr_tamano - espacioUsado) <<" Bytes\033[0m"<< endl;
                cout << "\033[96mEspacio necesario para partición extendida:  " << BytesExtend << " Bytes\033[0m" << endl;
                if((mbrE.mbr_tamano - espacioUsado) >= BytesExtend){
                    if(!(ExisteParticion(rutacompletaExtend,name))){
                        if(mbrE.disk_fit == 'F'){
                            mbrE.mbr_partition[ContadorPart].part_type = 'E';
                            mbrE.mbr_partition[ContadorPart].part_fit = fitpartExtend;
                            if(ContadorPart == 0){
                                mbrE.mbr_partition[ContadorPart].part_start = sizeof(mbrE);
                            }else{
                                mbrE.mbr_partition[ContadorPart].part_start =  mbrE.mbr_partition[ContadorPart-1].part_start + mbrE.mbr_partition[ContadorPart-1].part_size;
                            }
                            mbrE.mbr_partition[ContadorPart].part_size = BytesExtend;
                            mbrE.mbr_partition[ContadorPart].part_status = '0';
                            strcpy(mbrE.mbr_partition[ContadorPart].part_name,name.c_str());
                            fseek(fileDisk,0,SEEK_SET);
                            fwrite(&mbrE,sizeof(MBR),1,fileDisk);
                            fseek(fileDisk, mbrE.mbr_partition[ContadorPart].part_start,SEEK_SET);
                            EBR ebr;
                            ebr.part_fit = fitpartExtend;
                            ebr.part_status = '0';
                            ebr.part_start = mbrE.mbr_partition[ContadorPart].part_start;
                            ebr.part_size = 0;
                            ebr.part_next = -1;
                            strcpy(ebr.part_name, "");
                            fwrite(&ebr,sizeof (EBR),1,fileDisk);
                            for(int i = 0; i < (BytesExtend - (int)sizeof(EBR)); i++){
                                fwrite(&buffer,1,1,fileDisk);
                            }
                            cout << "\n \033[96mParticion EXTENDIDA creada con exito de tipo FIRST FIT\033[0m"<< endl;
                        }else if(mbrE.disk_fit == 'B'){
                            int MejorPosicion = ContadorPart;
                            for(int i = 0; i < 4; i++){
                                if(mbrE.mbr_partition[i].part_start == -1 || (mbrE.mbr_partition[i].part_status == '1' && mbrE.mbr_partition[i].part_size>=BytesExtend)){
                                    if(i != ContadorPart){
                                        if(mbrE.mbr_partition[MejorPosicion].part_size > mbrE.mbr_partition[i].part_size){
                                            MejorPosicion = i;
                                            break;
                                        }
                                    }
                                }
                            }
                            mbrE.mbr_partition[MejorPosicion].part_type = 'E';
                            mbrE.mbr_partition[MejorPosicion].part_fit = fitpartExtend;
                            if(MejorPosicion == 0){
                                mbrE.mbr_partition[MejorPosicion].part_start = sizeof(mbrE);
                            }else{
                                mbrE.mbr_partition[MejorPosicion].part_start =  mbrE.mbr_partition[MejorPosicion-1].part_start + mbrE.mbr_partition[MejorPosicion-1].part_size;
                            }
                            mbrE.mbr_partition[MejorPosicion].part_size = BytesExtend;
                            mbrE.mbr_partition[MejorPosicion].part_status = '0';
                            strcpy(mbrE.mbr_partition[MejorPosicion].part_name,name.c_str());
                            fseek(fileDisk,0,SEEK_SET);
                            fwrite(&mbrE,sizeof(MBR),1,fileDisk);
                            fseek(fileDisk, mbrE.mbr_partition[MejorPosicion].part_start,SEEK_SET);
                            EBR ebr;
                            ebr.part_fit = fitpartExtend;
                            ebr.part_status = '0';
                            ebr.part_start = mbrE.mbr_partition[MejorPosicion].part_start;
                            ebr.part_size = 0;
                            ebr.part_next = -1;
                            strcpy(ebr.part_name, "");
                            fwrite(&ebr,sizeof (EBR),1,fileDisk);
                            for(int i = 0; i < (BytesExtend - (int)sizeof(EBR)); i++){
                                fwrite(&buffer,1,1,fileDisk);
                            }
                            cout<< "\n \033[96mParticion EXTENDIDA creada con exito de tipo BEST FIT\033[0m"<< endl;
                        }else if(mbrE.disk_fit == 'W'){
                            int  PeorPosicion= ContadorPart;
                            for(int i = 0; i < 4; i++){
                                if(mbrE.mbr_partition[i].part_start == -1 || (mbrE.mbr_partition[i].part_status == '1' && mbrE.mbr_partition[i].part_size>=BytesExtend)){
                                    if(i != ContadorPart){
                                        if(mbrE.mbr_partition[PeorPosicion].part_size < mbrE.mbr_partition[i].part_size){
                                            PeorPosicion = i;
                                            break;
                                        }
                                    }
                                }
                            }
                            mbrE.mbr_partition[PeorPosicion].part_type = 'E';
                            mbrE.mbr_partition[PeorPosicion].part_fit = fitpartExtend;
                            if(PeorPosicion == 0){
                                mbrE.mbr_partition[PeorPosicion].part_start = sizeof(mbrE);
                            }else{
                                mbrE.mbr_partition[PeorPosicion].part_start =  mbrE.mbr_partition[PeorPosicion-1].part_start + mbrE.mbr_partition[PeorPosicion-1].part_size;
                            }
                            mbrE.mbr_partition[PeorPosicion].part_size = BytesExtend;
                            mbrE.mbr_partition[PeorPosicion].part_status = '0';
                            strcpy(mbrE.mbr_partition[PeorPosicion].part_name,name.c_str());
                            fseek(fileDisk,0,SEEK_SET);
                            fwrite(&mbrE,sizeof(MBR),1,fileDisk);
                            fseek(fileDisk, mbrE.mbr_partition[PeorPosicion].part_start,SEEK_SET);
                            EBR ebr;
                            ebr.part_fit = fitpartExtend;
                            ebr.part_status = '0';
                            ebr.part_start = mbrE.mbr_partition[PeorPosicion].part_start;
                            ebr.part_size = 0;
                            ebr.part_next = -1;
                            strcpy(ebr.part_name, "");
                            fwrite(&ebr,sizeof (EBR),1,fileDisk);
                            for(int i = 0; i < (BytesExtend - (int)sizeof(EBR)); i++){
                                fwrite(&buffer,1,1,fileDisk);
                            }
                            cout<< "\n \033[96mParticion EXTENDIDA creada con exito de tipo WORST FIT\033[0m"<< endl;
                        }
                    }else{
                        cout << "\033[91mError: La particion ya Exist!!\033[0m" << endl;
                    }
                }else{
                    cout << "\033[91mError: No hay tanto espacio disponible, intenta con una particion que no exceda el espacio libre\033[0m" << endl;
                }
            }else{
                cout << "\033[91mError: Ya se posee el numero maximo de particiones que son 4, elimina una para poder crear particion lógica!!\033[0m" << endl;
            }
        }else{
            cout << "\033[91mError: Ya Exist una partición de tipo EXTENDIDA\033[0m" << endl;
        }
    fclose(fileDisk);
    }else{
        cout << "\033[91mError: No se pudo abrir el disco, es posible que no exista!!\033[0m" << endl;
    }
}
void ParticionLogica(int size,char unit,string ruta,char fit,string name){
    char fitPartLogic = 0;
    char UnitPartLogic = 0;
    string Rutacompleta = path + ruta;
    int tamLogic;
    char buffer = '1';

    if(fit != 0)
        fitPartLogic = fit;
    else
        fitPartLogic = 'W';
    if(unit != 0){
        UnitPartLogic = unit;
        if(UnitPartLogic == 'B'){
            tamLogic = size;
        }else if(UnitPartLogic == 'K'){
            tamLogic = size * 1024;
        }else if(UnitPartLogic=='M'){
            tamLogic = size*1024*1024;
        }
    }else{
        tamLogic = size * 1024;
    }

    FILE *filelogic;
    MBR mbr;
    if((filelogic = fopen(Rutacompleta.c_str(), "rb+"))){
        int PosicionExtendida = -1;
        fseek(filelogic,0,SEEK_SET);
        fread(&mbr,sizeof(MBR),1,filelogic);
        for(int i = 0; i < 4; i++){
            if(mbr.mbr_partition[i].part_type == 'E'){
                PosicionExtendida = i;
                break;
            }
        }
        if(!ExisteParticion(Rutacompleta,name)){
            if(PosicionExtendida != -1){
                EBR ebr;
                fseek(filelogic,mbr.mbr_partition[PosicionExtendida].part_start,SEEK_SET);
                fread(&ebr, sizeof(EBR),1,filelogic);
                if(ebr.part_size == 0){
                    if(mbr.mbr_partition[PosicionExtendida].part_size < tamLogic){
                        cout << "\033[91mError: No hay suficiente espacio en la Extendida para esta partición lógica.\033[0m" << endl;
                    }else{
                        ebr.part_status = '0';
                        ebr.part_fit = fitPartLogic;
                        ebr.part_start = ftell(filelogic) - sizeof(EBR);
                        ebr.part_size = tamLogic;
                        ebr.part_next = -1;
                        strcpy(ebr.part_name, name.c_str());
                        fseek(filelogic, mbr.mbr_partition[PosicionExtendida].part_start ,SEEK_SET);
                        fwrite(&ebr,sizeof(EBR),1,filelogic);
                        cout << "\n\033[96m Particion LOGICA creada con exito!!!\033[0m "<< endl;
                    }
                }else{
                    while((ebr.part_next != -1) && (ftell(filelogic) < (mbr.mbr_partition[PosicionExtendida].part_size + mbr.mbr_partition[PosicionExtendida].part_start))){
                        fseek(filelogic,ebr.part_next,SEEK_SET);
                        fread(&ebr,sizeof(EBR),1,filelogic);
                    }
                    int espacioNecesario = ebr.part_start + ebr.part_size + tamLogic;
                    if(espacioNecesario <= (mbr.mbr_partition[PosicionExtendida].part_size + mbr.mbr_partition[PosicionExtendida].part_start)){
                        ebr.part_next = ebr.part_start + ebr.part_size;
                        fseek(filelogic,ftell(filelogic) - sizeof (EBR),SEEK_SET);
                        fwrite(&ebr, sizeof(EBR),1 ,filelogic);
                        fseek(filelogic,ebr.part_start + ebr.part_size, SEEK_SET);
                        ebr.part_status = 0;
                        ebr.part_fit = fitPartLogic;
                        ebr.part_start = ftell(filelogic);
                        ebr.part_size = tamLogic;
                        ebr.part_next = -1;
                        strcpy(ebr.part_name,name.c_str());
                        fwrite(&ebr,sizeof(EBR),1,filelogic);
                        cout << "\033[96m Particion LOGICA creada con exito \033[0m"<< endl;
                    }else{
                        cout << "\033[91mError: Espacio insuficiente, la lógica sobrepasa el espacio disponible!!\033[0m" << endl;
                    }
                }
            }else{
                cout << "\033[91mError: No se puede crear partición lógica sin extendida, crear partición EXTENDIDA antes.\033[0m" << endl;
            }
        }else{
            cout << "\033[91mError: Ya Exist una partición con el mismo nombre, cambielo e intente de nuevo.\033[0m" << endl;
        }

    fclose(filelogic);
    }else{
        cout << "\033[91mError: No se ha encontrado el disco, es probable que no exista.\033[0m" << endl;
    }

}
vector<nodoParticionMontada> arreglonodos;
int FindLetra(string name, string path){
    int letra = 'a';
    if(arreglonodos.size()==0){//Si esta vacio
        return letra;//retornamos a;
    }else if(arreglonodos.size()!=0){//Si no esta vacio
        for(int i = 0; i < arreglonodos.size();i++){
            if((arreglonodos[i].path==path)&&(arreglonodos[i].name==name)){
                return -1;
            }else{
                if(arreglonodos[i].path == path){
                    return arreglonodos[i].letra;
                }else if(letra <= arreglonodos[i].letra){
                    letra++;
                }
            }
        }
    }
    return letra; 
}
int FindNumero(string name,string path){
    int numero = 1;
    for(int i = 0; i < arreglonodos.size();i++){
        if((arreglonodos[i].numero == numero) && arreglonodos[i].path == path){
            numero++;
        }
    }
    return numero;
}

void ImprimirParticinesMontadas(){
    cout<<"\033[96m***** Particiones Montadas *****"<<endl;
    cout<<"***    Nombre  |    ID       ***\033[0m"<<endl;
    for(int i=0;i < arreglonodos.size();i++){
        cout<<"***  "<<arreglonodos[i].name<<"  |  vd"<<arreglonodos[i].letra<<arreglonodos[i].numero<<endl;
        cout<<"********************************"<<endl;
    }
}

void MontarParticion(string name, string pathr){
    string pathcompleta = path + pathr;
    int NumeroPrimariaOrExtendida = FindPrimariaYExtendida(pathcompleta,name);
    if(NumeroPrimariaOrExtendida != -1){
        FILE *filedisk;
        if((filedisk = fopen(pathcompleta.c_str(),"rb+"))){
            MBR mbr;
            fseek(filedisk, 0, SEEK_SET);
            fread(&mbr, sizeof(MBR),1,filedisk);
            mbr.mbr_partition[NumeroPrimariaOrExtendida].part_status = '2';
            fseek(filedisk,0,SEEK_SET);
            fwrite(&mbr,sizeof(MBR),1,filedisk);
            fclose(filedisk);
            int letra = FindLetra(name,pathcompleta);
            if(letra == -1){
                cout << "\033[31mError: la particion ya esta montada.\033[0m" << endl;
            }else{
                int num = FindNumero(name,pathcompleta);
                char LetraCasteada = static_cast<char>(letra);
                string id = "vd";
                id += LetraCasteada + to_string(num);
                nodoParticionMontada newNodo;
                newNodo.path = pathcompleta;
                newNodo.name = name;
                newNodo.letra = LetraCasteada;
                newNodo.numero = num;
                arreglonodos.push_back(newNodo);
                cout << "\033[94mParticion montada con exito.\033[0m" << endl;
                ImprimirParticinesMontadas();
            }
        }else{
            cout << "\033[31mErro: no se encuentra el disco.\033[0m" << endl;
        }
    }else{//Posiblemente logica
        int NumeroLogica = FindLogic(pathcompleta,name);
        if(NumeroLogica != -1){
            FILE *filedisk;
            if((filedisk = fopen(pathcompleta.c_str(), "rb+"))){
                EBR ebr;
                fseek(filedisk, NumeroLogica, SEEK_SET);
                fread(&ebr, sizeof(EBR),1,filedisk);
                ebr.part_status = '2';
                fseek(filedisk,NumeroLogica,SEEK_SET);
                fwrite(&ebr,sizeof(EBR),1, filedisk);
                fclose(filedisk);
                int letra = FindLetra(name,pathcompleta);
                if(letra == -1){
                    cout << "\033[31mERROR: La particion ya esta montada.\033[0m" << endl;
                }else{
                    int num = FindNumero(name,pathcompleta);
                    char LetraCasteada = static_cast<char>(letra);
                    string id = "vd";
                    id += LetraCasteada + to_string(num);
                    nodoParticionMontada newNodo;
                    newNodo.path = pathcompleta;
                    newNodo.name = name;
                    newNodo.letra = LetraCasteada;
                    newNodo.numero = num;
                    arreglonodos.push_back(newNodo);
                    cout << "\033[94mParticion montada con exito.\033[0m" << endl;
                    ImprimirParticinesMontadas();
                }
            }else{
                cout << "\033[31mERROR: no se encuentra el disco.\033[0m" << endl;
            }
        }else{
            cout << "\033[31mERROR: no se encuentra la particion a montar.\033[0m" << endl;
        }
    }
}

int FindPrimariaYExtendida(string direccion, string nombre){
    string auxPath = direccion;
    string auxName = nombre;
    FILE *filedisk;
    if((filedisk = fopen(auxPath.c_str(),"rb+"))){
        MBR mbr;
        fseek(filedisk,0,SEEK_SET);
        fread(&mbr,sizeof(MBR),1,filedisk);
        for(int i = 0; i < 4; i++){
            if(mbr.mbr_partition[i].part_status != '1'){
                if(strcmp(mbr.mbr_partition[i].part_name,auxName.c_str()) == 0){
                    return i;
                }
            }
        }

    }
    return -1;
}

int FindLogic(string direccion, string nombre){
    string auxPath = direccion;
    string auxName = nombre;
    FILE *filedisk;
    if((filedisk = fopen(auxPath.c_str(),"rb+"))){
        int extendida = -1;
        MBR mbr;
        fseek(filedisk,0,SEEK_SET);
        fread(&mbr,sizeof(MBR),1,filedisk);
        for(int i = 0; i < 4; i++){
            if(mbr.mbr_partition[i].part_type == 'E'){
                extendida = i;
                break;
            }
        }
        if(extendida != -1){
            EBR ebr;
            fseek(filedisk, mbr.mbr_partition[extendida].part_start,SEEK_SET);
            while(fread(&ebr,sizeof(EBR),1,filedisk)!=0 && (ftell(filedisk) < mbr.mbr_partition[extendida].part_start + mbr.mbr_partition[extendida].part_size)){
                if(strcmp(ebr.part_name, auxName.c_str()) == 0){
                    return (ftell(filedisk) - sizeof(EBR));
                }
            }
        }
        fclose(filedisk);
    }
    return -1;
}

void DesmontarParticion(string id){
    string idtmp;
    bool flag=false;
    for(int i=0;i < arreglonodos.size();i++){
        string vd="VD";
        string letra="";
        letra = toupper(arreglonodos[i].letra);
        string numero = to_string(arreglonodos[i].numero);
        idtmp = vd+letra+numero;
        if(idtmp==id){ 
            arreglonodos.erase(arreglonodos.begin()+ i);  
            cout<<"\033[92m Partición desmontada con éxito. \033[0m"<<endl;
            ImprimirParticinesMontadas();
            flag = true;
            break;    
        }
    }
    if(!flag){
        cout<<"\033[91m Error: No se encontro la partición que desea desmontar!!!\n Las particiones montadas son: \033[0m"<<endl;
        ImprimirParticinesMontadas();
    }
}

void FormatEXT2(int inicio ,int tamano,string direccion){
    double n = 1+(tamano - static_cast<int>(sizeof(SuperBloque))-1)/(4+static_cast<int>(sizeof(Tabla_Inodos)) +3*static_cast<int>(sizeof(Bloque_Carpetas)));
    int ContadorEstructuras = static_cast<int>(floor(n));//Calculo para el numero de inodos
    int ContadorBloques = 3*ContadorEstructuras;
    //Asignamo todos los parametros del SB bloque
    SuperBloque SBlock;
    SBlock.s_filesystem_type = 2;
    SBlock.s_inodes_count = ContadorEstructuras;
    SBlock.s_blocks_count = ContadorBloques;
    SBlock.s_free_blocks_count = ContadorBloques -2;
    SBlock.s_free_inodes_count = ContadorEstructuras -2;
    SBlock.s_mtime = time(nullptr);//como aun no se ha montado
    SBlock.s_umtime = 0;
    SBlock.s_mnt_count = 0;
    SBlock.s_magic = 0xEF53;
    SBlock.s_inode_size = sizeof(Tabla_Inodos);
    SBlock.s_block_size = sizeof(Bloque_Archivos);
    SBlock.s_first_ino = 2;
    SBlock.s_first_blo = 2;
    //EStablecemos inicios de las diferente estructuras tanto de bitmaps como de inodos
    SBlock.s_bm_inode_start = inicio + static_cast<int>(sizeof(SuperBloque));
    SBlock.s_bm_block_start = inicio + static_cast<int>(sizeof(SuperBloque)) + ContadorEstructuras;
    SBlock.s_inode_start = inicio + static_cast<int>(sizeof (SuperBloque)) + ContadorEstructuras + ContadorBloques;
    SBlock.s_block_start = inicio + static_cast<int>(sizeof(SuperBloque)) + ContadorEstructuras + ContadorBloques + (static_cast<int>(sizeof(Tabla_Inodos))*ContadorEstructuras);
    //Declaramos la tabla de nodos y el bloque de carpetas
    Tabla_Inodos InodoBlock;
    Bloque_Carpetas Block;

    char buffer = '0';
    char buffer2 = '1';
    char buffer3 = '2';

    FILE *fileDisk = fopen(direccion.c_str(),"rb+");
    fseek(fileDisk,inicio,SEEK_SET);//Establecemos la posicion de donde escribiremos
    fwrite(&SBlock,sizeof(SuperBloque),1,fileDisk);//Escribimos el superbloque
    for(int i = 0; i < ContadorEstructuras; i++){
        fseek(fileDisk,SBlock.s_bm_inode_start + i,SEEK_SET);//Establecemos posicion
        fwrite(&buffer,sizeof(char),1,fileDisk);//Escribimos el bitmap de inodos en todas las estructuras
    }
    fseek(fileDisk,SBlock.s_bm_inode_start,SEEK_SET);
    fwrite(&buffer2,sizeof(char),1,fileDisk);// llenamos para users
    fwrite(&buffer2,sizeof(char),1,fileDisk);
    for(int i = 0; i < ContadorBloques; i++){
        fseek(fileDisk,SBlock.s_bm_block_start + i,SEEK_SET);//Establecemos posicion
        fwrite(&buffer,sizeof(char),1,fileDisk);//Escibimos los bloques
    }
    fseek(fileDisk,SBlock.s_bm_block_start,SEEK_SET);
    fwrite(&buffer2,sizeof(char),1,fileDisk);
    fwrite(&buffer3,sizeof(char),1,fileDisk);// llenamos para users

    //Llenamos para el usuario root
    InodoBlock.i_uid = 1;
    InodoBlock.i_gid = 1;
    InodoBlock.i_size = 0;
    InodoBlock.i_atime = time(nullptr);
    InodoBlock.i_ctime = time(nullptr);
    InodoBlock.i_mtime = time(nullptr);
    InodoBlock.i_block[0] = 0;
    for(int i = 1; i < 15;i++)
        InodoBlock.i_block[i] = -1;
    InodoBlock.i_type = '0';
    InodoBlock.i_perm = 664;
    fseek(fileDisk,SBlock.s_inode_start,SEEK_SET);
    fwrite(&InodoBlock,sizeof(Tabla_Inodos),1,fileDisk);
    //Copiamos para el bloque de la carpeta root con la carpeta actual
    strcpy(Block.b_content[0].b_name,".");
    Block.b_content[0].b_inodo=0;   
    //Ahora para la capeta padre
    strcpy(Block.b_content[1].b_name,"..");
    Block.b_content[1].b_inodo=0;
    //Copiamos al archivo
    strcpy(Block.b_content[2].b_name,"users.txt");
    Block.b_content[2].b_inodo=1;

    strcpy(Block.b_content[3].b_name,".");
    Block.b_content[3].b_inodo=-1;
    fseek(fileDisk,SBlock.s_block_start,SEEK_SET);
    fwrite(&Block,sizeof(Bloque_Carpetas),1,fileDisk);
    //Inodo para los usuarios del archivo txt
    InodoBlock.i_uid = 1;
    InodoBlock.i_gid = 1;
    InodoBlock.i_size = 27;
    InodoBlock.i_atime = time(nullptr);
    InodoBlock.i_ctime = time(nullptr);
    InodoBlock.i_mtime = time(nullptr);
    InodoBlock.i_block[0] = 1;
    for(int i = 1; i < 15;i++){
        InodoBlock.i_block[i] = -1;
    }
    InodoBlock.i_type = '1';
    InodoBlock.i_perm = 755;
    fseek(fileDisk,SBlock.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
    fwrite(&InodoBlock,sizeof(Tabla_Inodos),1,fileDisk);
    //Bloque de archivos para el usuario root
    Bloque_Archivos archivo;
    memset(archivo.b_content,0,sizeof(archivo.b_content));
    strcpy(archivo.b_content,"1,G,root\n1,U,root,root,123\n");
    fseek(fileDisk,SBlock.s_block_start + static_cast<int>(sizeof(Bloque_Carpetas)),SEEK_SET);
    fwrite(&archivo,sizeof(Bloque_Archivos),1,fileDisk);
    
    cout << "\033[96mFormateo de tipo EXT2 realizada con éxito\033[0m" << endl;
    fclose(fileDisk);//Cerramos el disco
}

void FormatEXT3(int inicio,int tamano ,string direccion){
    double n = 1+(tamano - static_cast<int>(sizeof(SuperBloque))-1)/(4 + static_cast<int>(sizeof(Journaling))+static_cast<int>(sizeof(Tabla_Inodos)) +3*static_cast<int>(sizeof(Bloque_Carpetas)));
    int ContadorEstructuras = static_cast<int>(floor(n));//Este tendra el bitmap de inodos
    int ContadorBloques = 3*ContadorEstructuras;//Este llevara el bitmap de bloques
    int super_size = static_cast<int>(sizeof(SuperBloque));
    int journal_size = static_cast<int>(sizeof(Journaling))*ContadorEstructuras;

    SuperBloque SBlock;
    SBlock.s_filesystem_type = 3;
    SBlock.s_inodes_count = ContadorEstructuras;
    SBlock.s_blocks_count = ContadorBloques;
    SBlock.s_free_blocks_count = ContadorBloques - 2;
    SBlock.s_free_inodes_count = ContadorEstructuras - 2;
    SBlock.s_mtime = time(nullptr);
    SBlock.s_umtime = 0;
    SBlock.s_mnt_count = 0;
    SBlock.s_magic = 0xEF53;
    SBlock.s_inode_size = sizeof(Tabla_Inodos);
    SBlock.s_block_size = sizeof(Bloque_Archivos);
    SBlock.s_first_ino = 2;
    SBlock.s_first_blo = 2;
    SBlock.s_bm_inode_start = inicio + super_size + journal_size;
    SBlock.s_bm_block_start = inicio + super_size + journal_size + ContadorEstructuras;
    SBlock.s_inode_start = inicio + super_size + journal_size + ContadorEstructuras + ContadorBloques;
    SBlock.s_block_start = inicio + super_size + journal_size + ContadorEstructuras + ContadorBloques + static_cast<int>(sizeof(Tabla_Inodos))*ContadorEstructuras;

    Tabla_Inodos InodoBlock;
    Bloque_Carpetas Block;

    char buffer = '0';
    char buffer2 = '1';
    char buffer3 = '2';

    FILE *fileDisk = fopen(direccion.c_str(),"rb+");

    //Posicionamos y escribimos para el superbloque 
    fseek(fileDisk,inicio,SEEK_SET);
    fwrite(&SBlock,sizeof(SuperBloque),1,fileDisk);
    //Recorremos todas las estructuras y escribimos para los bitmaps de inodos
    for(int i = 0; i < ContadorEstructuras; i++){
        fseek(fileDisk,SBlock.s_bm_inode_start + i,SEEK_SET);
        fwrite(&buffer,sizeof(char),1,fileDisk);
    }
    //Escribimos el bit para el root / y para users txt
    fseek(fileDisk,SBlock.s_bm_inode_start,SEEK_SET);
    fwrite(&buffer2,sizeof(char),1,fileDisk);
    fwrite(&buffer2,sizeof(char),1,fileDisk);
    //Recorremos los bloques y escribimos los bitmaps de los mismos
    for(int i = 0; i < ContadorBloques; i++){
        fseek(fileDisk,SBlock.s_bm_block_start + i,SEEK_SET);
        fwrite(&buffer,sizeof(char),1,fileDisk);
    }
    //bitmap para root / y para users txt
    fseek(fileDisk,SBlock.s_bm_block_start,SEEK_SET);
    fwrite(&buffer2,sizeof(char),1,fileDisk);
    fwrite(&buffer3,sizeof(char),1,fileDisk);
    //Inodo para la carpeta raiz del root
    InodoBlock.i_uid = 1;
    InodoBlock.i_gid = 1;
    InodoBlock.i_size = 0;
    InodoBlock.i_atime = time(nullptr);
    InodoBlock.i_ctime = time(nullptr);
    InodoBlock.i_mtime = time(nullptr);
    InodoBlock.i_block[0] = 0;
    for(int i = 1; i < 15;i++){
        InodoBlock.i_block[i] = -1;
    }
    InodoBlock.i_type = '0';
    InodoBlock.i_perm = 664;
    fseek(fileDisk,SBlock.s_inode_start,SEEK_SET);
    fwrite(&InodoBlock,sizeof(Tabla_Inodos),1,fileDisk);
    //Bloque para la carpeta root

    //Escribimos en la caprta actual
    strcpy(Block.b_content[0].b_name,".");
    Block.b_content[0].b_inodo=0;

    //Copiamos a la carpeta padre
    strcpy(Block.b_content[1].b_name,"..");
    Block.b_content[1].b_inodo=0;

    
    strcpy(Block.b_content[2].b_name,"users.txt");
    Block.b_content[2].b_inodo=1;

    strcpy(Block.b_content[3].b_name,".");
    Block.b_content[3].b_inodo=-1;
    fseek(fileDisk,SBlock.s_block_start,SEEK_SET);
    fwrite(&Block,sizeof(Bloque_Carpetas),1,fileDisk);
    //Llenamos parametros para los inodos de los usuarios
    InodoBlock.i_uid = 1;
    InodoBlock.i_gid = 1;
    InodoBlock.i_size = 27;
    InodoBlock.i_atime = time(nullptr);
    InodoBlock.i_ctime = time(nullptr);
    InodoBlock.i_mtime = time(nullptr);
    InodoBlock.i_block[0] = 1;
    for(int i = 1; i < 15;i++){
        InodoBlock.i_block[i] = -1;
    }
    InodoBlock.i_type = '1';
    InodoBlock.i_perm = 755;
    fseek(fileDisk,SBlock.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
    fwrite(&InodoBlock,sizeof(Tabla_Inodos),1,fileDisk);
    //Bloque para los archivos de los usuarios
    Bloque_Archivos archivo;
    memset(archivo.b_content,0,sizeof(archivo.b_content));
    strcpy(archivo.b_content,"1,G,root\n1,U,root,root,123\n");
    fseek(fileDisk,SBlock.s_block_start + static_cast<int>(sizeof(Bloque_Carpetas)),SEEK_SET);
    fwrite(&archivo,sizeof(Bloque_Archivos),1,fileDisk);

    cout << "\033[96mFormateo de tipo EXT3 realizada con éxito\033[0m" << endl;
    fclose(fileDisk);//Cerramos el disco
}
void ComandoMKFS(string id,string type,int fs){
    string idtmp;
    bool flag=false;
    for(int i=0;i < arreglonodos.size();i++){
        string vd="VD";
        string letra="";
        letra = toupper(arreglonodos[i].letra);
        string numero = to_string(arreglonodos[i].numero);
        idtmp = vd+letra+numero;
        if(idtmp==id){ 
            int contador = FindPrimariaYExtendida(arreglonodos[i].path,arreglonodos[i].name);
                if(contador != -1){
                    MBR mbr;
                    FILE *fileDisk = fopen(arreglonodos[i].path.c_str(),"rb+");
                    fread(&mbr,sizeof(MBR),1,fileDisk);
                    int inicio = mbr.mbr_partition[contador].part_start;
                    int tamano = mbr.mbr_partition[contador].part_size;
                    if(fs == 2){
                        FormatEXT2(inicio,tamano,arreglonodos[i].path);
                        Mkfsids.push_back(id);
                    }
                    else if(fs==3){
                        FormatEXT3(inicio,tamano,arreglonodos[i].path);
                        Mkfsids.push_back(id);
                    }
                    fclose(fileDisk);
                }else{
                    contador = FindLogic(arreglonodos[i].path,arreglonodos[i].name);
                }            
            flag = true;
            break;    
        }
    }
    if(!flag){
        cout<<"\033[91m Error: No se encontro la partición montada con ese id!!!\n Las particiones montadas son: \033[0m"<<endl;
    }
}

/*
Funciones y metodos para el manejo del login y todas sus validaciones.
*/
int FindGrupo(string grupo){
    FILE *FileDisk = fopen(SesionActual.direccion.c_str(),"rb+");

    char arreglochar[400] = "\0";
    SuperBloque SB;
    Tabla_Inodos Inodo;

    fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);
    //Leemos el Inodo del archivo users.txt
    fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)), SEEK_SET);
    fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);

    for(int i = 0; i < 15; i++){
        if(Inodo.i_block[i] != -1){
            Bloque_Archivos archivo;
            fseek(FileDisk,SB.s_block_start,SEEK_SET);
            for(int j = 0; j <= Inodo.i_block[i]; j++){
                fread(&archivo,sizeof(Bloque_Archivos),1,FileDisk);
            }
            strcat(arreglochar,archivo.b_content);
        }
    }

    fclose(FileDisk);

    char *finalcadena;
    char *llave = strtok_r(arreglochar,"\n",&finalcadena);
    while(llave != nullptr){
        char id[2];
        char tipo[2];
        char group[12];
        char *Finalllave;
        char *llavei = strtok_r(llave,",",&Finalllave);
        strcpy(id,llavei);
        if(strcmp(id,"0") != 0){//Verificar que no sea un U/G eliminado
            llavei = strtok_r(nullptr,",",&Finalllave);
            strcpy(tipo,llavei);
            if(strcmp(tipo,"G") == 0){
                strcpy(group,Finalllave);
                if(strcmp(group,grupo.c_str()) == 0)
                    return atoi(id);
            }
        }
        llave = strtok_r(nullptr,"\n",&finalcadena);
    }

    return -1;
}

int VerificarParametros(string user,string password,string direccion){
    FILE *FileDisk = fopen(direccion.c_str(),"rb+");

    char arreglochar[400] = "\0";
    SuperBloque SB;
    Tabla_Inodos Inodo;

    fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);
    //Leemos el Inodo del archivo users.txt
    fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
    fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);

    for(int i = 0; i < 15; i++){
        if(Inodo.i_block[i] != -1){
            Bloque_Archivos archivo;
            fseek(FileDisk,SB.s_block_start,SEEK_SET);
            for(int j = 0; j <= Inodo.i_block[i]; j++){
                fread(&archivo,sizeof(Bloque_Archivos),1,FileDisk);
            }
            strcat(arreglochar,archivo.b_content);
        }
    }

    fclose(FileDisk);

    char *finalcadena;
    char *llave = strtok_r(arreglochar,"\n",&finalcadena);
    while(llave != nullptr){
        char id[2];
        char tipo[2];
        string group;
        char user_[12];
        char password_[12];
        char *Finalllave;
        char *llavei = strtok_r(llave,",",&Finalllave);
        strcpy(id,llavei);
        if(strcmp(id,"0") != 0){//Verificar que no sea un U/G eliminado
            llavei=strtok_r(nullptr,",",&Finalllave);
            strcpy(tipo,llavei);
            if(strcmp(tipo,"U") == 0){
                llavei = strtok_r(nullptr,",",&Finalllave);
                group = llavei;
                llavei = strtok_r(nullptr,",",&Finalllave);
                strcpy(user_,llavei);
                llavei = strtok_r(nullptr,",",&Finalllave);
                strcpy(password_,llavei);
                if(strcmp(user_,user.c_str()) == 0){
                    if(strcmp(password_,password.c_str()) == 0){
                        SesionActual.direccion = direccion;
                        SesionActual.IDU = atoi(id);
                        SesionActual.IDG = FindGrupo(group);
                        return 1;
                    }else{
                        return 2;
                    }
                        
                }
            }
        }
        llave = strtok_r(nullptr,"\n",&finalcadena);
    }

    return 0;
}

char ObtenerFit(string path,string nombre){
    FILE *FileDisk;
    if((FileDisk = fopen(path.c_str(),"rb+"))){
        int numeroExtendida = -1;
        MBR mbr;
        fseek(FileDisk,0,SEEK_SET);
        fread(&mbr,sizeof(MBR),1,FileDisk);
        for(int i = 0; i < 4; i++){
            if(mbr.mbr_partition[i].part_type == 'E'){
                numeroExtendida = i;
                break;
            }
        }
        if(numeroExtendida != -1){
            EBR ebr;
            fseek(FileDisk, mbr.mbr_partition[numeroExtendida].part_start,SEEK_SET);
            while(fread(&ebr,sizeof(EBR),1,FileDisk)!=0 && (ftell(FileDisk) < mbr.mbr_partition[numeroExtendida].part_start + mbr.mbr_partition[numeroExtendida].part_size)){
                if(strcmp(ebr.part_name, nombre.c_str()) == 0){
                    return ebr.part_fit;
                }
            }
        }
        fclose(FileDisk);
    }
    return -1;
}


int VerificarLogin(string path,string nombre,string user,string password){
    int contador = FindPrimariaYExtendida(path,nombre);
    if(contador != -1){
        MBR mbr;
        SuperBloque SB;
        Tabla_Inodos Inodo;
        FILE *FileDisk = fopen(path.c_str(),"rb+");
        fread(&mbr,sizeof(MBR),1,FileDisk);
        fseek(FileDisk,mbr.mbr_partition[contador].part_start,SEEK_SET);
        fread(&SB,sizeof(SuperBloque),1,FileDisk);
        fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
        fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);
        fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
        Inodo.i_atime = time(nullptr);
        fwrite(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);
        fclose(FileDisk);
        SesionActual.PosicionInicialSB = mbr.mbr_partition[contador].part_start;
        SesionActual.fit = mbr.mbr_partition[contador].part_fit;
        SesionActual.PosicionInicialJournal = mbr.mbr_partition[contador].part_start + static_cast<int>(sizeof(SuperBloque));
        SesionActual.FS = SB.s_filesystem_type;
        return VerificarParametros(user,password, path);
    }else{
        contador = FindLogic(path, nombre);
        if(contador != -1){
            SuperBloque SB;
            Tabla_Inodos Inodo;
            FILE *FileDisk = fopen(path.c_str(),"rb+");
            fseek(FileDisk,contador + static_cast<int>(sizeof(EBR)),SEEK_SET);
            fread(&SB,sizeof(SuperBloque),1,FileDisk);
            fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
            fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);
            fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
            Inodo.i_atime = time(nullptr);
            fwrite(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);
            fclose(FileDisk);
            SesionActual.PosicionInicialSB = contador + static_cast<int>(sizeof(EBR));
            SesionActual.fit = ObtenerFit(path,nombre);
            return VerificarParametros(user,password,path);
        }
    }
    return 0;
}

void login(string usuario,string contrasena,string id){
    string idtmp;
    bool flag=false;
    bool mkfstrue=false;
    for(int i=0;i < arreglonodos.size();i++){
        string vd="VD";
        string letra="";
        letra = toupper(arreglonodos[i].letra);
        string numero = to_string(arreglonodos[i].numero);
        idtmp = vd+letra+numero;
        for(int i=0;i<Mkfsids.size();i++){
            if(Mkfsids[i]==idtmp){
                mkfstrue=true;
            }
        }
        if(idtmp==id){
            flag=true;
            if(!SesionActiva){
                if(mkfstrue){
                int retornoVerificacion = VerificarLogin(arreglonodos[i].path,arreglonodos[i].name,usuario,contrasena);
                if(retornoVerificacion == 1){
                    SesionActiva = true;
                    cout << "\033[96m Sesion iniciada con exito\033[0m" << endl;
                }else if(retornoVerificacion == 2)
                    cout << "\033[91m Error: Contrasena incorrecta\033[0m" << endl;
                else if(retornoVerificacion == 0)
                    cout << "\033[91m Error: Usuario no encontrado \033[0m" << endl;   
                break;    
            }else{
                cout << "\033[91m Error:Se debe formatear la particion con formato EXT2 o EXT3 antes de intentar iniciar sesion\033[0m" << endl;
            }             
        }else{
            cout<<"\033[91m Error: Se a encontrado una seccion activa porfavor haga un LOGOUT y vuelva a intentar loguearse \033[0m"<<endl;
        }
        }
    }
    if(!flag){
        cout<<"\033[91m Error: No se encontro la partición montada con ese id!!!\n Las particiones montadas son: \033[0m"<<endl;
    }
}

void logout(){
    if(SesionActiva){
        SesionActiva = false;
        SesionActual.IDU=-1;
        SesionActual.direccion="";
        SesionActual.PosicionInicialSB=-1;
        cout<<"\033[96mSesion cerrada con exito!!\033[0m"<<endl;
    }else{
        cout<<"\033[91m No hay sesion activa la cual se pueda cerrar!!\033[0m"<<endl;
    }
}
string EliminarComillas(string cadena){
    char replacement[] = "";
    string result = cadena.replace(0,1, replacement);
    result = result.replace(cadena.size()-1,1,replacement);
    return result;
}
void MKGRP(string name){
    string nombreGrupo = EliminarComillas(name); 
    if(SesionActiva){
        if(SesionActual.IDU == 1 && SesionActual.IDG == 1){//Usuario root
            if(nombreGrupo.length() <= 10){
                int grupo = FindGrupo(nombreGrupo);
                if(grupo == -1){
                    int idGrp = ObtenerIDG();
                    string nuevoGrupo = to_string(idGrp)+",G,"+nombreGrupo+"\n";
                    AddUserstxt(nuevoGrupo);
                    cout << "\033[96m El grupo se creo exitosamente :D \033[0m "<< endl;
                    //Guardamos el registro en el journal si es un EXT3
                    if(SesionActual.FS ==3){
                        char aux[64];
                        char operacion[10];
                        char content[2];
                        strcpy(aux,nuevoGrupo.c_str());
                        strcpy(operacion,"mkgrp");
                        memset(content,0,2);
                        SaveJournaling(operacion,0  ,0,aux,content);
                    }
                }else
                    cout << "\033[91m Error: Este grupo ya Exist, cambiele nombre porfavor \033[0m" << endl;
            }else
                cout << "\033[91m Error: El nombre es muy largo, maximo 10 caracteres \033[0m " << endl;
        }else
            cout << " \033[91m Error: No esta logeado con el usuario ROOT, accion no posible!! \033[0m " << endl;
    }else
        cout << "\033[91m Error: Sesion no iniciada \033[0m " << endl;
}
int ObtenerIDG(){
    FILE *FileDisk = fopen(SesionActual.direccion.c_str(),"rb+");

    char arreglochar[400] = "\0";
    int Identificadortmp = -1;
    SuperBloque SB;
    Tabla_Inodos Inodo;
    fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);
    fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)), SEEK_SET);//Posicionamos para el Inodo
    fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);//Leemos el Inodo.. del txt

    for(int i = 0; i < 15; i++){
        if(Inodo.i_block[i] != -1){
            Bloque_Archivos file;
            fseek(FileDisk,SB.s_block_start,SEEK_SET);
            for(int j = 0; j <= Inodo.i_block[i]; j++){
                fread(&file,sizeof(Bloque_Archivos),1,FileDisk);
            }
            strcat(arreglochar,file.b_content);
        }
    }

    fclose(FileDisk);

    char *FinCadena;
    char *llave = strtok_r(arreglochar,"\n",&FinCadena);
    while(llave != nullptr){
        char id[2];
        char tipo[2];
        char *finllave;
        char *token2 = strtok_r(llave,",",&finllave);
        strcpy(id,token2);
        if(strcmp(id,"0") != 0){//Verificar que no sea un U/G eliminado
            token2 = strtok_r(nullptr,",",&finllave);
            strcpy(tipo,token2);
            if(strcmp(tipo,"G") == 0){
                Identificadortmp = atoi(id);
            }

        }
        llave = strtok_r(nullptr,"\n",&FinCadena);
    }
    return ++Identificadortmp;
}
void SaveJournaling(char* operacion,int tipo,int permisos,char *nombre,char *content){
    SuperBloque SB;
    Journaling registroEXT3;
    strcpy(registroEXT3.Journal_tipo_operacion,operacion);
    registroEXT3.Journal_tipo = tipo;
    strcpy(registroEXT3.Journal_nombre,nombre);
    strcpy(registroEXT3.Journal_contenido,content);
    registroEXT3.Journal_fecha = time(nullptr);
    registroEXT3.Journal_propietario = SesionActual.IDU;
    registroEXT3.Journal_permisos = permisos;
    FILE *FileDisk = fopen(SesionActual.direccion.c_str(),"rb+");
    Journaling registroEXT3tmp;
    bool fin = false;
    fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);
    int JournalingIni = SesionActual.PosicionInicialSB + static_cast<int>(sizeof(SuperBloque));
    int JournalingFin = SB.s_bm_inode_start;
    fseek(FileDisk,JournalingIni,SEEK_SET);
    while((ftell(FileDisk) < JournalingFin) && !fin){
        fread(&registroEXT3tmp,sizeof(Journaling),1,FileDisk);
        if(registroEXT3tmp.Journal_tipo != 0 && registroEXT3tmp.Journal_tipo != 1)
            fin = true;
    }
    fseek(FileDisk,ftell(FileDisk)- static_cast<int>(sizeof(Journaling)),SEEK_SET);
    fwrite(&registroEXT3,sizeof(Journaling),1,FileDisk);
    fclose(FileDisk);
}
void AddUserstxt(string datos){
    FILE *FileDisk = fopen(SesionActual.direccion.c_str(), "rb+");

    SuperBloque SB;
    Tabla_Inodos Inodo;
    Bloque_Archivos ArchivoBloque;
    int NumBloque = 0;

    fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);
    fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)), SEEK_SET);
    fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);

    for(int i = 0; i < 12; i++){
        if(Inodo.i_block[i] != -1)
            NumBloque = Inodo.i_block[i];
    }

    fseek(FileDisk,SB.s_block_start + static_cast<int>(sizeof(Bloque_Archivos))*NumBloque,SEEK_SET);
    fread(&ArchivoBloque,sizeof(Bloque_Archivos),1,FileDisk);
    int EspacioUsado = static_cast<int>(strlen(ArchivoBloque.b_content));
    int EspacioLibre = 63 - EspacioUsado;

    if(datos.length() <= EspacioLibre){
        strcat(ArchivoBloque.b_content,datos.c_str());
        fseek(FileDisk,SB.s_block_start + static_cast<int>(sizeof(Bloque_Archivos))*NumBloque,SEEK_SET);
        fwrite(&ArchivoBloque,sizeof(Bloque_Archivos),1,FileDisk);
        fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
        fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);
        Inodo.i_size = Inodo.i_size + datos.length();
        Inodo.i_mtime = time(nullptr);
        fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
        fwrite(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);
    }else{
        string tmp = "";
        string tmp1 = "";
        int i = 0;

        for(i = 0; i < EspacioLibre; i++)
            tmp += datos.at(i);

        for(; i < datos.length(); i++)
            tmp1  += datos.at(i);

        /*
        En el primer bloque guardamos lo que le quepa
        */
        strcat(ArchivoBloque.b_content,tmp.c_str());
        fseek(FileDisk,SB.s_block_start + static_cast<int>(sizeof(Bloque_Archivos))*NumBloque,SEEK_SET);
        fwrite(&ArchivoBloque,sizeof(Bloque_Archivos),1,FileDisk);
        Bloque_Archivos auxArchivo;
        strcpy(auxArchivo.b_content,tmp1.c_str());
        int bit = FindBit(FileDisk,'B',SesionActual.fit);
        /*
        Escribimos en el bitmap y en la tabla de bloques el bloque
        */

        fseek(FileDisk,SB.s_bm_block_start + bit,SEEK_SET);
        fputc('2',FileDisk);
        fseek(FileDisk,SB.s_block_start + (static_cast<int>(sizeof(Bloque_Archivos))*bit),SEEK_SET);
        fwrite(&auxArchivo,sizeof(Bloque_Archivos),1,FileDisk);

        /*
        Escribimos el Inodo que se cambio
        */
        fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
        fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);
        Inodo.i_size = Inodo.i_size + datos.length();
        Inodo.i_mtime = time(nullptr);
        Inodo.i_block[NumBloque] = bit;
        fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
        fwrite(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);
        /*
        Escribimos los bloques que quedan libres y seteamos el primero disponible ahra
        */
        SB.s_first_blo = SB.s_first_blo + 1;
        SB.s_free_blocks_count = SB.s_free_blocks_count - 1;
        fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
        fwrite(&SB,sizeof(SuperBloque),1,FileDisk);
    }
    fclose(FileDisk);
}
int FindBit(FILE *FileDisk, char tipo, char fit){
    SuperBloque SB;
    int BitmapInicio = 0;
    char Bitaux = '0';
    int BitDisponible = -1;
    int BitmapTamanio = 0;

    fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);

    if(tipo == 'I'){
        BitmapTamanio = SB.s_inodes_count;
        BitmapInicio = SB.s_bm_inode_start;
    }else if(tipo == 'B'){
        BitmapTamanio = SB.s_blocks_count;
        BitmapInicio = SB.s_bm_block_start;
    }

    if(fit == 'F'){     //Primer ajuste
        for(int i = 0; i < BitmapTamanio; i++){
            fseek(FileDisk,BitmapInicio + i,SEEK_SET);
            Bitaux = static_cast<char>(fgetc(FileDisk));
            if(Bitaux == '0'){
                BitDisponible = i;
                return BitDisponible;
            }
        }

        if(BitDisponible == -1)
            return -1;

    }else if(fit == 'B'){   //Mejor ajuste
        int libres = 0;
        int auxLibres = -1;

        /*
        Hacemos el recorrido inicial
        */
        for(int i = 0; i < BitmapTamanio; i++){
            fseek(FileDisk,BitmapInicio + i,SEEK_SET);
            Bitaux = static_cast<char>(fgetc(FileDisk));
            if(Bitaux == '0'){
                libres++;
                if(i+1 == BitmapTamanio){
                    if(auxLibres == -1 || auxLibres == 0)
                        auxLibres = libres;
                    else{
                        if(auxLibres > libres)
                            auxLibres = libres;
                    }
                    libres = 0;
                }
            }else if(Bitaux == '1'){
                if(auxLibres == -1 || auxLibres == 0)
                    auxLibres = libres;
                else{
                    if(auxLibres > libres)
                        auxLibres = libres;
                }
                libres = 0;
            }
        }

        for(int i = 0; i < BitmapTamanio; i++){
            fseek(FileDisk,BitmapInicio + i, SEEK_SET);
            Bitaux = static_cast<char>(fgetc(FileDisk));
            if(Bitaux == '0'){
                libres++;
                if(i+1 == BitmapTamanio)
                    return ((i+1)-libres);
            }else if(Bitaux == '1'){
                if(auxLibres == libres)
                    return ((i+1) - libres);
                libres = 0;
            }
        }

        return -1;

    }else if(fit == 'W'){//Peor ajuste
        int libres = 0;
        int auxLibres = -1;

        for (int i = 0; i < BitmapTamanio; i++) {
            fseek(FileDisk,BitmapInicio + i, SEEK_SET);
            Bitaux = static_cast<char>(fgetc(FileDisk));
            if(Bitaux == '0'){
                libres++;
                if(i+1 == BitmapTamanio){
                    if(auxLibres == -1 || auxLibres == 0)
                        auxLibres = libres;
                    else {
                        if(auxLibres < libres)
                            auxLibres = libres;
                    }
                    libres = 0;
                }
            }else if(Bitaux == '1'){
                if(auxLibres == -1 || auxLibres == 0)
                    auxLibres = libres;
                else{
                    if(auxLibres < libres)
                        auxLibres = libres;
                }
                libres = 0;
            }
        }

        for (int i = 0; i < BitmapTamanio; i++) {
            fseek(FileDisk,BitmapInicio + i, SEEK_SET);
            Bitaux = static_cast<char>(fgetc(FileDisk));
            if(Bitaux == '0'){
                libres++;
                if(i+1 == BitmapTamanio)
                    return ((i+1) - libres);
            }else if(Bitaux == '1'){
                if(auxLibres == libres)
                    return ((i+1) - libres);
                libres = 0;
            }
        }

        return -1;
    }

    return 0;
}
void RMGRP(string name){
    string nombreGrupo = EliminarComillas(name);
    if(SesionActiva){
        if(SesionActual.IDU == 1 && SesionActual.IDG == 1){//Usuario root
            int grupo = FindGrupo(nombreGrupo);
            if(grupo != -1){
                REMOVERGRUPO(nombreGrupo);
            }else
                cout << "\033[91m Error: Grupo inexistente \033[0m " << endl;
        }else
           cout << "\033[91m Error: Comando solo peude ser ejecutado por el usuario ROOT \033[0m " << endl;
    }else
        cout << "\033[91m Error: Sesion no iniciada \033[0m " << endl;
}
void REMOVERGRUPO(string name){
    FILE *FileDisk = fopen(SesionActual.direccion.c_str(),"rb+");

    SuperBloque SB;
    Tabla_Inodos Inodo;
    Bloque_Archivos archivo;

    int columna = 1;
    char actual;
    int posicion = 0;
    int BloqueContador = 0;
    int id = -1;
    char tipo = '\0';
    string grupo = "";
    string palabra = "";
    bool tmp = false;


    fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);
    //Nos posicionamos en el Inodo del archivo users.txt
    fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
    fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);

    for (int i = 0; i < 12; i++) {
        if(Inodo.i_block[i] != -1){
            fseek(FileDisk,SB.s_block_start + static_cast<int>(sizeof(Bloque_Archivos))*Inodo.i_block[i],SEEK_SET);
            fread(&archivo,sizeof(Bloque_Archivos),1,FileDisk);
            for(int j = 0; j < 63; j++){
                actual = archivo.b_content[j];
                if(actual=='\n'){
                    if(tipo == 'G'){
                        grupo = palabra;
                        if(strcmp(grupo.c_str(),name.c_str()) == 0){
                            fseek(FileDisk,SB.s_block_start+static_cast<int>(sizeof(Bloque_Archivos))*BloqueContador,SEEK_SET);
                            fread(&archivo,sizeof(Bloque_Carpetas),1,FileDisk);
                            archivo.b_content[posicion] = '0';
                            fseek(FileDisk,SB.s_block_start+static_cast<int>(sizeof(Bloque_Archivos))*BloqueContador,SEEK_SET);
                            fwrite(&archivo,sizeof(Bloque_Archivos),1,FileDisk);
                            cout << "\033[96m El grupo se elimino exitosamente!! :D \033[0m" << endl;
                            tmp = true;
                            break;
                        }
                    }
                    columna = 1;
                    palabra = "";
                }else if(actual != ','){
                    palabra += actual;
                    columna++;
                }else if(actual == ','){
                    if(columna == 2){
                        id = atoi(palabra.c_str());
                        posicion = j-1;
                        BloqueContador = Inodo.i_block[i];
                    }
                    else if(columna == 4)
                        tipo = palabra[0];
                    columna++;
                    palabra = "";
                }
            }
            if(tmp)
                break;
        }
    }

    fclose(FileDisk);
}



void MKUSR(string user, string pass, string group){
    if(user.length() <= 10){
        if(pass.length() <= 10){
            if(group.length() <= 10){
                if(SesionActiva){
                    if(SesionActual.IDU == 1 && SesionActual.IDG == 1){//Usuario root
                        if(FindGrupo(group) != -1){
                            if(!buscarUsuario(user)){
                                int id = getID_usr();
                                string datos = to_string(id) + ",U,"+group+","+user+","+pass+"\n";
                                AddUserstxt(datos);
                                cout << "\033[96m El usuario se elimino exitosamente!! :D \033[0m"  << endl;
                                //Guardamos el registro en el journal si es un sistema EXT3
                                if(SesionActual.FS ==3){
                                    char aux[64];
                                    char operacion[10];
                                    char content[2];
                                    strcpy(aux,datos.c_str());
                                    strcpy(operacion,"mkusr");
                                    memset(content,0,2);
                                    SaveJournaling(operacion,0,0,aux,content);
                                }
                            }else
                                cout << "\033[91m Error: Este usuario ya existe \033[0m " <<endl;
                        }else
                            cout << "\033[91m Error: No se encuentra el grupo de este usuario \033[0m  " << endl;
                    }else
                        cout << "\033[91m Error: Este comando solamente es para el usuario ROOT lo siento :c \033[0m " << endl;
                }else
                    cout << "\033[91m Error: Debe iniciar sesión para poder ejecutar el comando \033[0m " << endl;
            }else
                cout << "\033[91m Error: El nombre del grupo excede los 10 caracteres permitidos ;( \033[0m " << endl;
        }else
            cout << "\033[91m Error: La contraseña excede los 10 caracters permitidos!! \033[0m " << endl;
    }else
        cout << "\033[91m Error: El nombre del usuario excede los 10 caracteres permitidos :c !\033[0m " << endl;
}
bool buscarUsuario(string name){
    FILE *FileDisk = fopen(SesionActual.direccion.c_str(),"rb+");

    char cadena[400] = "\0";
    SuperBloque SB;
    Tabla_Inodos Inodo;

    fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);
    //Nos posicionamos en el Inodo del archivo users.txt
    fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)), SEEK_SET);
    fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);

    for(int i = 0; i < 15; i++){
        if(Inodo.i_block[i] != -1){
            Bloque_Archivos archivo;
            fseek(FileDisk,SB.s_block_start,SEEK_SET);
            for(int j = 0; j <= Inodo.i_block[i]; j++){
                fread(&archivo,sizeof(Bloque_Archivos),1,FileDisk);
            }
            strcat(cadena,archivo.b_content);
        }
    }

    fclose(FileDisk);

    char *end_str;
    char *llave = strtok_r(cadena,"\n",&end_str);
    while(llave != nullptr){
        char id[2];
        char tipo[2];
        char user[12];
        char *end_token;
        char *token2 = strtok_r(llave,",",&end_token);
        strcpy(id,token2);
        if(strcmp(id,"0") != 0){//Verificar que no sea un U/G eliminado
            token2 = strtok_r(nullptr,",",&end_token);
            strcpy(tipo,token2);
            if(strcmp(tipo,"U") == 0){
                token2 = strtok_r(nullptr,",",&end_token);
                token2 = strtok_r(nullptr,",",&end_token);
                strcpy(user,token2);

                if(strcmp(user,name.c_str()) == 0)
                    return true;
            }
        }
        llave = strtok_r(nullptr,"\n",&end_str);
    }

    return false;
}
int getID_usr(){
    FILE *FileDisk = fopen(SesionActual.direccion.c_str(),"rb+");

    char cadena[400] = "\0";
    int res = 0;
    SuperBloque SB;
    Tabla_Inodos Inodo;

    fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);
    //Nos posicionamos en el Inodo del archivo users.txt
    fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)), SEEK_SET);
    fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);

    for(int i = 0; i < 15; i++){
        if(Inodo.i_block[i] != -1){
            Bloque_Archivos archivo;
            fseek(FileDisk,SB.s_block_start,SEEK_SET);
            for(int j = 0; j <= Inodo.i_block[i]; j++){
                fread(&archivo,sizeof(Bloque_Archivos),1,FileDisk);
            }
            strcat(cadena,archivo.b_content);
        }
    }

    fclose(FileDisk);

    char *end_str;
    char *llave = strtok_r(cadena,"\n",&end_str);
    while(llave != nullptr){
        char id[2];
        char tipo[2];
        char *end_token;
        char *token2 = strtok_r(llave,",",&end_token);
        strcpy(id,token2);
        if(strcmp(id,"0") != 0){//Verificar que no sea un U/G eliminado
            token2 = strtok_r(nullptr,",",&end_token);
            strcpy(tipo,token2);
            if(strcmp(tipo,"U") == 0)
                res++;
        }
        llave = strtok_r(nullptr,"\n",&end_str);
    }
    return ++res;
}


void RMUSR(string userName){
    if(SesionActiva){
        if(SesionActual.IDU == 1 && SesionActual.IDG == 1){
            if(buscarUsuario(userName)){
                eliminarUsuario(userName);
            }else
                cout << "\033[91m Error: El usuario no se encontro, no existe.\033[0m " << endl;
        }else
           cout << "\033[91m Error: Lo sentimos pero este comando solo esta permitido para el usuario ROOT \033[0m " << endl;
    }else
        cout << "\033[91m Error: Debe iniciar sesión para ejecutar comando (user ROOT) !!\033[0m " << endl;
}
void eliminarUsuario(string name){
    FILE *FileDisk = fopen(SesionActual.direccion.c_str(),"rb+");

    SuperBloque SB;
    Tabla_Inodos Inodo;
    Bloque_Archivos archivo;

    int col = 1;
    char actual;
    string palabra = "";
    int posicion = 0;
    int numBloque = 0;
    int id = -1;
    char tipo = '\0';
    string grupo = "";
    string usuario = "";
    bool flag = false;

    fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);
    fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
    fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);

    for (int i = 0; i < 12; i++) {
        if(Inodo.i_block[i] != -1){
            fseek(FileDisk,SB.s_block_start + static_cast<int>(sizeof(Bloque_Archivos))*Inodo.i_block[i],SEEK_SET);
            fread(&archivo,sizeof(Bloque_Archivos),1,FileDisk);
            for(int j = 0; j < 63; j++){
                actual = archivo.b_content[j];
                if(actual=='\n'){
                    if(tipo == 'U'){
                        if(strcmp(usuario.c_str(),name.c_str()) == 0){
                            fseek(FileDisk,SB.s_block_start+static_cast<int>(sizeof(Bloque_Archivos))*numBloque,SEEK_SET);
                            fread(&archivo,sizeof(Bloque_Archivos),1,FileDisk);
                            archivo.b_content[posicion] = '0';
                            fseek(FileDisk,SB.s_block_start+static_cast<int>(sizeof(Bloque_Archivos))*numBloque,SEEK_SET);
                            fwrite(&archivo,sizeof(Bloque_Archivos),1,FileDisk);
                            cout << "\033[96m Se eliminó el usuario con éxito :D \033[0m " << endl;
                            flag = true;
                            break;
                        }
                        usuario = "";
                        grupo = "";
                    }
                    col = 1;
                    palabra = "";
                }else if(actual != ','){
                    palabra += actual;
                    col++;
                }else if(actual == ','){
                    if(col == 2){
                        id = atoi(palabra.c_str());
                        posicion = j-1;
                        numBloque = Inodo.i_block[i];
                    }
                    else if(col == 4)
                        tipo = palabra[0];
                    else if(grupo == "")
                        grupo = palabra;
                    else if(usuario == "")
                        usuario = palabra;
                    col++;
                    palabra = "";
                }
            }
            if(flag)
                break;
        }
    }

    fclose(FileDisk);
}



void cambiarPermisosRecursivo(FILE* archivo, int n, int permisos){
    SuperBloque SB;
    Tabla_Inodos Inodo;
    Bloque_Carpetas carpeta;
    char b ='0';

    fseek(archivo,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,archivo);
    fseek(archivo,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*n,SEEK_SET);
    fread(&Inodo,sizeof(Tabla_Inodos),1,archivo);
    Inodo.i_perm = permisos;
    fseek(archivo,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*n,SEEK_SET);
    fwrite(&Inodo,sizeof(Tabla_Inodos),1,archivo);

    for(int i = 0; i < 15; i++){
        if(Inodo.i_block[i] != -1){
            fseek(archivo,SB.s_bm_block_start + Inodo.i_block[i],SEEK_SET);
            b = static_cast<char>(fgetc(archivo));
            if(b == '1'){
                fseek(archivo,SB.s_block_start + static_cast<int>(sizeof(Bloque_Carpetas))*Inodo.i_block[i],SEEK_SET);
                fread(&carpeta,sizeof(Bloque_Carpetas),1,archivo);
                for(int j = 0; j < 4; j++){
                    if(carpeta.b_content[j].b_inodo != -1){
                        if(strcmp(carpeta.b_content[j].b_name,".")!=0 &&  strcmp(carpeta.b_content[j].b_name,"..")!=0)
                            cambiarPermisosRecursivo(archivo,carpeta.b_content[j].b_inodo,permisos);
                    }
                }
            }
        }
    }
}

void CHMOD(string pathg, string ugo,bool flagR){
    if(SesionActiva){

        int user =stoi(to_string(ugo.at(0)).c_str()) - '0';//Obtener el valor numero de un char
        int grupo = stoi(to_string(ugo.at(0)).c_str()) - '0';
        int otros = stoi(to_string(ugo.at(0)).c_str()) - '0';
        if((user >= 0 && user <= 7) && (grupo >= 0 && grupo <= 7) && (otros >= 0 && otros <= 7)){
            char auxPath[500];
            strcpy(auxPath,pathg.c_str());
            FILE *FileDisk = fopen(SesionActual.direccion.c_str(),"rb+");
            SuperBloque SB;
            Tabla_Inodos Inodo;
            int Exist = buscarCarpetaArchivo(FileDisk,auxPath);
            fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
            fread(&SB,sizeof(SuperBloque),1,FileDisk);
            fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*Exist,SEEK_SET);
            fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);
            if(Exist != -1){
                if((SesionActual.IDU ==1 && SesionActual.IDG == 1) || SesionActual.IDU == Inodo.i_uid){
                    if(flagR)
                        cambiarPermisosRecursivo(FileDisk,Exist,stoi(ugo));
                    else{
                        Inodo.i_perm = stoi(ugo);
                        fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*Exist,SEEK_SET);
                        fwrite(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);
                    }
                    cout << "\033[96m Permisos cambiados exitosamente!! \033[0m " << endl;
                }else
                    cout << "\033[91m Error: Para cambiar los permisos debes ser propietario de la capeta o el archivo, sino estar logeado con el usuario root \033[0m " << endl;
            }else
                cout << "\033[91m Error: La ruta no existe. \033[0m " << endl;
            fclose(FileDisk);
        }else
            cout << "\033[91m Error: Algún dígito del UGO esta fuera de rango, solo esta permitido del 0 hasta el 7 \033[0m "<< endl;
    }else
        cout << "\033[91m Error: Debe iniciar sesión para ejecutar este comando...\033[0m " << endl;
}

int buscarCarpetaArchivo(FILE *archivo, char* path){
    SuperBloque SB;
    Tabla_Inodos Inodo;
    Bloque_Carpetas carpeta;
    BloqueDeApuntadores Punter;

    vector<string> lista;
    char *llave = strtok(path,"/");
    int cont = 0;
    int numInodo = 0;

    while(llave != nullptr){
        lista.push_back(llave);
        cont++;
        llave = strtok(nullptr,"/");
    }

    fseek(archivo,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,archivo);
    //Posicionamos para el bit donde inicia el inodo
    numInodo = SB.s_inode_start; 

    for (int cont2 = 0; cont2 < cont; cont2++) {
        fseek(archivo,numInodo,SEEK_SET);
        fread(&Inodo,sizeof(Tabla_Inodos),1,archivo);
        int siguiente = 0;
        for(int i = 0; i < 15; i++){
            if(Inodo.i_block[i] != -1){
                //Apuntador directo
                int byteBloque = byteInodoBloque(archivo,Inodo.i_block[i],'2');
                fseek(archivo,byteBloque,SEEK_SET);
                if(i < 12){
                    fread(&carpeta,sizeof(Bloque_Carpetas),1,archivo);
                    for (int j = 0; j < 4; j++) {
                        if((cont2 == cont - 1) && (strcasecmp(carpeta.b_content[j].b_name,lista.at(cont2).c_str()) == 0)){//Haceos match con la carpeta
                            return carpeta.b_content[j].b_inodo;
                        }else if((cont2 != cont - 1) && (strcasecmp(carpeta.b_content[j].b_name,lista.at(cont2).c_str()) == 0)){
                            numInodo = byteInodoBloque(archivo,carpeta.b_content[j].b_inodo,'1');
                            siguiente = 1;
                            break;
                        }
                    }
                }else if(i == 12){
                    //Apuntador inidrecto
                    fread(&Punter,sizeof(BloqueDeApuntadores),1,archivo);
                    for(int j = 0; j < 16; j++){
                        if(Punter.b_pointers[j] != -1){
                            byteBloque = byteInodoBloque(archivo,Punter.b_pointers[j],'2');
                            fseek(archivo,byteBloque,SEEK_SET);
                            fread(&carpeta,sizeof(Bloque_Carpetas),1,archivo);
                            for (int k = 0; k < 4; k++) {
                                if((cont2 == cont - 1) && (strcasecmp(carpeta.b_content[k].b_name,lista.at(cont2).c_str()) == 0)){//Haceos match con la carpeta
                                    return carpeta.b_content[k].b_inodo;
                                }else if((cont2 != cont - 1) && (strcasecmp(carpeta.b_content[k].b_name,lista.at(cont2).c_str()) == 0)){
                                    numInodo = byteInodoBloque(archivo,carpeta.b_content[k].b_inodo,'1');
                                    siguiente = 1;
                                    break;
                                }
                            }
                            if(siguiente == 1)
                                break;
                        }
                    }
                }else if(i == 13){

                }else if(i == 14){

                }
                if(siguiente == 1)
                    break;
            }
        }
    }

    return -1;
}
int byteInodoBloque(FILE *archivo,int pos, char tipo){
    SuperBloque SB;
    fseek(archivo,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,archivo);
    if(tipo == '1'){
        return (SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*pos);
    }else if(tipo == '2')
        return (SB.s_block_start + static_cast<int>(sizeof(Bloque_Carpetas))*pos);
    return 0;
}

int main(int argc, char const *argv[])
{
    cout<<"\033[92m********************************************"<<endl;
    cout<<"**          HARD DISK SIMULATION          **"<<endl;
    cout<<"**               201905554                **"<<endl;
    cout<<"******************************************** \033[0m\n"<<endl;
    char Linea_Comando[200];
    
    while((string)Linea_Comando!="EXIT"){
        cout<<"\033[96mCommand :: \033[0m";
        cin.getline(Linea_Comando,200,'\n');
        char lineaSincasteo[200];
        strcpy(lineaSincasteo,Linea_Comando);      
        string tmp=CastearMayuscula(Linea_Comando);//Casteamos a mayuscula toda la linea para evitarnos problemas del case-insensitive.
        vector<string> ls= SplitSpace(tmp);
        if(ls[0]=="EXEC"){
            vector<string> tmp;
            tmp = Split(ls[1],"~:~");
            if(tmp[0]=="-PATH"){
                string ruta;
                ruta = path+tmp[1];
                leerscript(ruta);
            }
        }
        else{
            EjecutarComando(lineaSincasteo);
        }

    }    
    return EXIT_SUCCESS;
}