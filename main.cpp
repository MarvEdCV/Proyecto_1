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
//Variables globales
//TODO:ARREGLAR PATH 
string path="/home/eduardo/Escritorio/ArchivosVacas/Proyecto_1/Proyecto_1";

//ESTRUCTURAS GENERALES
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
void CrearDisco(DiscoD op){
cout<<"\033[92m************************CREANDO DISCO*************************\033[0m\n"<<endl;
    string s = path + op.path;//url raiz + url de la entrada
    char sc[s.size() + 1];
    strcpy(sc, s.c_str());
    FILE *file=NULL;
    file=fopen(sc,"r"); //r= read = sera verdadero si el disco ya existe
    if(file!=NULL){
        cout<<"\033[91mYa existe el disco\033[0m"<<endl;
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
    string token;
    vector<string> res;

    while ((pos_end = cadena.find (delimitador, pos_start)) != string::npos) {
        token = cadena.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
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
                    cout<<"\033[91mError: La particion ya existe!!\033[0m"<<endl;
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
                        cout << "\033[91mError: La particion ya existe!!\033[0m" << endl;
                    }
                }else{
                    cout << "\033[91mError: No hay tanto espacio disponible, intenta con una particion que no exceda el espacio libre\033[0m" << endl;
                }
            }else{
                cout << "\033[91mError: Ya se posee el numero maximo de particiones que son 4, elimina una para poder crear particion lógica!!\033[0m" << endl;
            }
        }else{
            cout << "\033[91mError: Ya existe una partición de tipo EXTENDIDA\033[0m" << endl;
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
            cout << "\033[91mError: Ya existe una partición con el mismo nombre, cambielo e intente de nuevo.\033[0m" << endl;
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
    bool flag=false;
    for(int i=0;i < arreglonodos.size();i++){
        string vd="VD";
        string letra="";
        letra = toupper(arreglonodos[i].letra);
        string numero = to_string(arreglonodos[i].numero);
        string idtmp = vd+letra+numero;
        if(idtmp==id){ 
            arreglonodos.erase(arreglonodos.begin()+ i+1);  
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
        string tmp=CastearMayuscula(Linea_Comando);//Casteamos a mayuscula toda la linea para evitarnos problemas del case-insensitive.
        vector<string> ls= SplitSpace(tmp);
        if(ls[0]=="EXEC"){
            vector<string> aux;
            aux = Split(ls[1],"~:~");
            if(aux[0]=="-PATH"){
                string ruta;
                ruta = path+aux[1];
                leerscript(ruta);
            }
        }
        else{
            EjecutarComando(Linea_Comando);
        }

    }    
    return EXIT_SUCCESS;
}