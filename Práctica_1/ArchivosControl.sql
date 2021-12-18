use Practica1;
load data local infile '/home/eduardo/Escritorio/ArchivosVacas/Proyecto_1/Proyecto_1/Práctica_1/ArchivosCSV/country_codes.tsv' 
into table tmp_countryCodes
character set UTF8
fields terminated by '\t' lines terminated by '\n' ignore 1 lines
(
@var1,
@var2,
@var3,
@var4,
@var5,
@var6,
@var7,
@var8,
@var9,
@var10,
@var11,
@var12,
@var13,
@var14,
@var15,
@var16,
@var17,
@var18
)
set name=if(@var1 = '',NULL,@var1),
iso2=if(@var2 = '',NULL,@var2),
name_name=if(@var3 = '',NULL,@var3),
name_aiddata_code=if(@var4 = '',NULL,@var4),
name_aiddata_name=if(@var5 = '',NULL,@var5),
name_cow_alpha=if(@var6 = '',NULL,@var6),
name_cow_numeric=if(@var7 = '',NULL,@var7),
name_fao_code=if(@var8 = '',NULL,@var8),
name_fips=if(@var9 = '',NULL,@var9),
name_geonames_id=if(@var10 = '',NULL,@var10),
name_imf_code=if(@var11 = '',NULL,@var11),
name_iso2=if(@var12 = '',NULL,@var12),
name_iso3=if(@var13 = '',NULL,@var13),
name_iso_numeric=if(@var14 = '',NULL,@var14),
name_oecd_code=if(@var15 = '',NULL,@var15),
name_oecd_name=if(@var16 = '',NULL,@var16),
name_un_code=if(@var17 = '',NULL,@var17),
name_wb_code=if(@var18 = '',NULL,@var18)
;


load data local infile '/home/eduardo/Escritorio/ArchivosVacas/Proyecto_1/Proyecto_1/Práctica_1/ArchivosCSV/geonames.csv' 
into table tmp_geoName
character set UTF8
fields terminated by ',' lines terminated by '\n' ignore 1 lines
(
@var1,
@var2,
@var3,
@var4,
@var5,
@var6,
@var7,
@var8,
@var9
)
set idGeoname=if(@var1 = '',NULL,@var1),
place_name=if(@var2 = '',NULL,@var2),
latitude=if(@var3 = '',NULL,@var3),
longitude=if(@var4 = '',NULL,@var4),
location_type_code=if(@var5 = '',NULL,@var5),
gazetteer_adm_code=if(@var6 = '',NULL,@var6),
gazetteer_adm_name=if(@var7 = '',NULL,@var7),
location_class=if(@var8 = '',NULL,@var8),
geographic_exactness=if(@var9 = '',NULL,@var9)
;

load data local infile '/home/eduardo/Escritorio/ArchivosVacas/Proyecto_1/Proyecto_1/Práctica_1/ArchivosCSV/level_1a.csv' 
into table tmp_Level
character set UTF8
fields terminated by ',' lines terminated by '\n' ignore 1 lines
(
@var1,
@var2,
@var3,
@var4,
@var5,
@var6,
@var7
)
set project_id = if(@var1 = '',NULL,@var1),
project_location_id = if(@var2 = '',NULL,@var2),
geoname_id=if(@var3 = '',NULL,@var3),
transactions_start_year = if(@var4 = '',NULL,@var4),
transactions_end_year_ = if(@var5 = '',NULL,@var5),
event_split_commitments = if(@var6 = '',NULL,@var6),
event_split_disbursements = if(@var7 = '',NULL,@var7)
;

load data local infile '/home/eduardo/Escritorio/ArchivosVacas/Proyecto_1/Proyecto_1/Práctica_1/ArchivosCSV/locations.csv' 
into table tmp_Locations
character set UTF8
fields terminated by ',' lines terminated by '\n' ignore 1 lines
(
@var1,
@var2
)
set location_type_code=if(@var1 = '',NULL,@var1),
location_type_name=if(@var2 = '',NULL,@var2)
;

load data local infile '/home/eduardo/Escritorio/ArchivosVacas/Proyecto_1/Proyecto_1/Práctica_1/ArchivosCSV/projects.csv' 
into table tmp_project
character set latin1
fields terminated by ',' lines terminated by '\n' ignore 1 lines
(
@var1,
@var2,
@var3,
@var4,
@var5,
@var6,
@var7,
@var8,
@var9,
@var10,
@var11,
@var12,
@var13,
@var14,
@var15,
@var16
)
set project_id=if(@var1 = '',NULL,@var1),
is_geocoded=if(@var2 = '',NULL,@var2),
project_title=if(@var3 = '',NULL,@var3),
start_actual_isodate=if(@var4 = '',NULL,@var4),
end_actual_isodate=if(@var5 = '',NULL,@var5),
donors=if(@var6 = '',NULL,@var6),
donors_iso3=if(@var7 = '',NULL,@var7),
recipients=if(@var8 = '',NULL,@var8),
recipients_iso3=if(@var9 = '',NULL,@var9),
ad_sector_codes=if(@var10 = '',NULL,@var10),
ad_sector_names=if(@var11 = '',NULL,@var11),
status=if(@var12 = '',NULL,@var12),
transactions_start_year=if(@var13 = '',NULL,@var13),
transactions_end_year=if(@var14 = '',NULL,@var14),
total_commitments=if(@var15 = '',NULL,@var15),
total_disbursements=if(@var16 = '',NULL,@var16)
;

load data local infile '/home/eduardo/Escritorio/ArchivosVacas/Proyecto_1/Proyecto_1/Práctica_1/ArchivosCSV/transactions.csv' 
into table tmp_transaction
character set UTF8
fields terminated by ',' lines terminated by '\n' ignore 1 lines
(
@var1,
@var2,
@var3,
@var4,
@var5,
@var6,
@var7
)
set id_transaction=if(@var1 = '',NULL,@var1),
idProject=if(@var2 = '',NULL,@var2),
transaction_isodate=if(@var3 = '',NULL,@var3),
transaction_year=if(@var4 = '',NULL,@var4),
transaction_value_code=if(@var5 = '',NULL,@var5),
transaction_currency=if(@var6 = '',NULL,@var6),
transaction_value=if(@var7 = '',NULL,@var7)
;

