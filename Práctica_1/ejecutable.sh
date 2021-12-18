
#!/bin/bash
mysql --user="root" --local_infile=1 --password="Marvinkata" --database="Practica1" --execute="use Practica1;"
mysql --user="root" --local_infile=1 --password="Marvinkata" --database="Practica1" --execute="source /home/eduardo/Escritorio/ArchivosVacas/Proyecto_1/Proyecto_1/Práctica_1/EliminacionTablas.sql;"
mysql --user="root" --local_infile=1 --password="Marvinkata" --database="Practica1" --execute="source /home/eduardo/Escritorio/ArchivosVacas/Proyecto_1/Proyecto_1/Práctica_1/CrearTablas.sql;"
mysql --user="root" --local_infile=1 --password="Marvinkata" --database="Practica1" --execute="source /home/eduardo/Escritorio/ArchivosVacas/Proyecto_1/Proyecto_1/Práctica_1/CrearTablasTemporales.sql;"
mysql --user="root" --local_infile=1 --password="Marvinkata" --database="Practica1" --execute="SET GLOBAL local_infile=1;"
mysql --user="root" --local_infile=1 --password="Marvinkata" --database="Practica1" --execute="source /home/eduardo/Escritorio/ArchivosVacas/Proyecto_1/Proyecto_1/Práctica_1/ArchivosControl.sql"
mysql --user="root" --local_infile=1 --password="Marvinkata" --database="Practica1" --execute="source /home/eduardo/Escritorio/ArchivosVacas/Proyecto_1/Proyecto_1/Práctica_1/LlenadoBase.sql;"