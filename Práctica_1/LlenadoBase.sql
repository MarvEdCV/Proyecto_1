
use Practica1;

SET FOREIGN_KEY_CHECKS = 0;

insert into countryCodes(
name,
iso2,
name_name,
name_aiddata_code,
name_aiddata_name,
name_cow_alpha,
name_cow_numeric,
name_fao_code,
name_fips,
name_geonames_id,
name_imf_code,
name_iso2,
name_iso3,
name_iso_numeric,
name_oecd_code,
name_oecd_name,
name_un_code,
name_wb_code
)
select distinct 
cc.name,
cc.iso2,
cc.name_name,
cc.name_aiddata_code,
cc.name_aiddata_name,
cc.name_cow_alpha,
cc.name_cow_numeric,
cc.name_fao_code,
cc.name_fips,
cc.name_geonames_id,
cc.name_imf_code,
cc.name_iso2,
cc.name_iso3,
cc.name_iso_numeric,
cc.name_oecd_code,
cc.name_oecd_name,
cc.name_un_code,
cc.name_wb_code
from tmp_countryCodes as cc;
SET FOREIGN_KEY_CHECKS = 1;




SET FOREIGN_KEY_CHECKS = 0;
insert into geoName(
idGeoname,
place_name,
latitude,
longitude,
location_type_code,
gazetteer_adm_code,
gazetteer_adm_name,
location_class,
geographic_exactness
)
select 
gn.idGeoname,
gn.place_name,
gn.latitude,
gn.longitude,
gn.location_type_code,
gn.gazetteer_adm_code,
gn.gazetteer_adm_name,
gn.location_class,
gn.geographic_exactness
from tmp_geoName as gn;
SET FOREIGN_KEY_CHECKS = 1;

SET FOREIGN_KEY_CHECKS = 0;
insert into Level(
project_id,
project_location_id,
geoname_id,
transactions_start_year,
transactions_end_year_,
event_split_commitments,
event_split_disbursements
)
select distinct 
lev.project_id,
lev.project_location_id,
lev.geoname_id,
lev.transactions_start_year,
lev.transactions_end_year_,
lev.event_split_commitments,
lev.event_split_disbursements
from tmp_Level as lev;
SET FOREIGN_KEY_CHECKS = 1;


SET FOREIGN_KEY_CHECKS = 0;
insert into Locations(
location_type_code,
location_type_name
)
select distinct
tmloc.location_type_code,
tmloc.location_type_name
from tmp_Locations as tmloc;
SET FOREIGN_KEY_CHECKS = 1;



SET FOREIGN_KEY_CHECKS = 0;
insert into project(
project_id,
is_geocoded,
project_title,
start_actual_isodate,
end_actual_isodate,
donors,
donors_iso3,
recipients,
recipients_iso3 ,
ad_sector_codes ,
ad_sector_names ,
status,
transactions_start_year,
transactions_end_year ,
total_commitments ,
total_disbursements
)
select distinct
tmp.project_id,
tmp.is_geocoded,
tmp.project_title,
tmp.start_actual_isodate,
tmp.end_actual_isodate,
tmp.donors ,
tmp.donors_iso3,
tmp.recipients ,
tmp.recipients_iso3 ,
tmp.ad_sector_codes ,
tmp.ad_sector_names ,
tmp.status ,
tmp.transactions_start_year,
tmp.transactions_end_year ,
tmp.total_commitments ,
tmp.total_disbursements 
from tmp_project as tmp;
SET FOREIGN_KEY_CHECKS = 1;


SET FOREIGN_KEY_CHECKS = 0;
insert into transaction(
id_transaction,
idProject,
transaction_isodate,
transaction_year ,
transaction_value_code ,
transaction_currency ,
transaction_value 
)
select distinct
trans.id_transaction,
trans.idProject,
trans.transaction_isodate,
trans.transaction_year ,
trans.transaction_value_code ,
trans.transaction_currency ,
trans.transaction_value 
from tmp_transaction as trans;
SET FOREIGN_KEY_CHECKS = 1;


