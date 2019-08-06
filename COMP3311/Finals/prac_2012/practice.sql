create or replace function fac(n integer) returns integer
as $$
declare 
	i integer;
	res integer;
begin
	if (n < 0) then
		return null;
	end if;
	
	-- now we know its positive;
	for i in 1..n loop
		res := res*i;
	end loop;
	return res;
end;
$$ language plpgsql;

create or replace function spread(text) returns text;
as $$
 
