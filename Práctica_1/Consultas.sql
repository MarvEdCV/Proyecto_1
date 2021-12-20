
create user 'empleado1'@'localhost' identified by 'empleado';
grant select on Practia1.* to empleado; 
create user 'Administrador1'@'localhost' identified by 'Administrador';
grant all on Practica1.* to Administrador; 

mysql --local-infile=1 -u root -p
SELECT User, Host FROM mysql.user;
mysql --local-infile=1 -u empleado1 -pempleado
show grants;
create database PruebaEmpleado;
mysql --local-infile=1 -u Administrador1 -pAdministrador
show grants;

select * from tmp_countryCodes;
select count(*) as Registros_CountryCodes from tmp_countryCodes;
select * from tmp_geoName;
select count(*) as Registros_Geoname from tmp_geoName;
select * from tmp_Level;
select count(*) as Registros_level from tmp_Level;
select * from tmp_Locations;
select count(*) as Registros_Locations from tmp_Locations;
select * from tmp_project;
select count(*) as Registros_Project from tmp_project;
select * from tmp_transaction;
select count(*) as Registros_Transaction from tmp_transaction;



#1CUANTOS PROYECTOS INICIARON EN 2011
select count(idProyecto) from project where year(start_actual_isodate)=2011;

#2¿Cuál fue el proyecto de mayor duración?
select project_id,datediff(end_actual_isodate,start_actual_isodate) as Duracion_Dias,project_title from project 
group by(project_id) 
order by 2 desc 
limit 1;

#3¿Cuál fue el proyecto de menor duración?
select project_id,datediff(end_actual_isodate,start_actual_isodate) as Duracion_Dias,project_title from project
WHERE start_actual_isodate is not null and
end_actual_isodate is not null
group by(project_id) 
order by 2 asc 
limit 1;

#4Top 5 de países con más proyectos asignados
select recipients_iso3, COUNT(recipients) as Proyecto_Asignado 
from project 
group BY recipients_iso3 
order by Proyecto_Asignado desc limit 5;


#5. Top 5 de países con menos proyectos asignados
 select recipients_iso3, COUNT(recipients) as Proyecto_Asignado 
 from project 
 group BY recipients_iso3 
 order by Proyecto_Asignado asc limit 5;

#6. ¿Cuál fue el proyecto con más costo?
select project_title as Proyeceto_mas_costoso , total_commitments from project
order by total_commitments  DESC
limit 1;

#7. ¿Cuál fue el proyecto menos costoso?
select project_title as Proyeceto_menos_costoso, total_commitments from project
where total_commitments>0 
order by total_commitments  ASC
limit 1;

#8Top 5 proyectos mas costosos..
select project_title,total_commitments from project
order by total_commitments desc
limit 5;

#9Costo Total de Todos los Proyectos
select sum(total_commitments) from project;

#10Top 3 de las Transacciones más Costosas RI-East Africa Public Health Laboratory Networking Project
select transaction.id_transaction,transaction.transaction_isodate,transaction.transaction_value
from transaction
join project on transaction.idProject = idProject
where project.project_title  like '%RI-East Africa Public Health Laboratory Networking Project%'
order by transaction.transaction_value desc
limit 3;