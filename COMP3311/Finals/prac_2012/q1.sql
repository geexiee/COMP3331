drop view if exists Q1;
create view Q1
as
select t.country as team, count(i.match) as matches
from   Teams t, Involves i
where  t.id = i.team
group  by t.country ;
