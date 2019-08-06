-- 1. List all the company names (and countries) that are incorporated outsideList all the company names (and countries) that are incorporated outside Australia.
    create or replace view Q1(Name, Country) as
	select name, country
	from company
	where country != 'Australia';

-- 2. List all the company codes that have more than five executive members on record (i.e., at least six).
    create or replace view Q2(Code) as
	select code 
	from executive 
	group by code 
	having count(*) > 5;

-- 3. List all the company names that are in the sector of "Technology"
    create or replace view Q3(Name) as
	select name
	from company comp, category cat
	where comp.code = cat.code and cat.sector = 'Technology';

-- 4. Find the number of Industries in each Sector
    create or replace view Q4(Sector, Number) as 
	select sector, count(distinct industry) 
	from category 
	group by sector;
-- 5. Find all the executives (i.e., their names) that are affiliated with companies in the sector of "Technology". If an executive is affiliated with more than one company, he/she is counted if one of these companies is in the sector of "Technology".
    create or replace view Q5(Name) as
	select person 
	from executive e, category c
	where e.code = c.code and c.sector = 'Technology';

-- 6. List all the company names in the sector of "Services" that are located in Australia with the first digit of their zip code being 2.
    create or replace view Q6(Name) as
	select name 
	from company c, category cat
	where c.code = cat.code and cat.sector = 'Services' and c.zip like '2%' and c.country = 'Australia';	

-- 7. Create a database view of the ASX table that contains previous Price, Price change (in amount, can be negative) and Price gain (in percentage, can be negative). (Note that the first trading day should be excluded in your result.) For example, if the PrevPrice is 1.00, Price is 0.85; then Change is -0.15 and Gain is -15.00 (in percentage but you do not need to print out the percentage sign).
	create or replace view prev as
	select "Date", code, volume, price, lag(price, 1) over (
		partition by code 
		order by "Date"
	)
	as prev_price
	from ASX;

	create or replace view Q7("Date", Code, Volume, PrevPrice, Price, Change, Gain) as
	select p."Date", p.code, p.volume, p.prev_price, p.price, p.price-p.prev_price as change,
	trunc(100*(p.price-p.prev_price)/p.prev_price, 16) as gain
	from prev p
	where p.prev_price is not null;

-- 8. Find the most active trading stock (the one with the maximum trading volume; if more than one, output all of them) on every trading day. Order your output by "Date" and then by Code.
	create or replace view volumes as 
	select a1."Date", max(a1.volume) as volume
	from ASX a1
	group by a1."Date"
	order by a1."Date";    

	create or replace view Q8("Date", Code, Volume) as
	select v."Date", a.code, v.volume 
	from ASX a, volumes v 
	group by v."Date", a.code, v.volume, a.volume 
	having a.volume = v.volume 
	order by v."Date";


-- 9. Find the number of companies per Industry. Order your result by Sector and then by Industry.
    create or replace view Q9(Sector, Industry, Number) as
	select c.sector, c.industry, count(distinct comp) as number
	from company comp, category c 
	where c.code = comp.code 
	group by sector, industry 
	order by sector, industry;

-- 10. List all the companies (by their Code) that are the only one in their Industry (i.e., no competitors).
	create or replace view cats as
	select cat.industry as ind, count(cat.industry) 
	from category cat
	group by cat.industry;

    create or replace view Q10(Code, Industry) as
	select c.code, cat.ind 
	from company c, cats cat, category cat1
	where cat.count = 1 and cat.ind = cat1.industry and cat1.code = c.code 
	order by c.code;
	
-- 11. List all sectors ranked by their average ratings in descending order. AvgRating is calculated by finding the average AvgCompanyRating for each sector (where AvgCompanyRating is the average rating of a company).
    create or replace view Q11(Sector, AvgRating) as
	select c.sector, avg(r.star) as rating 
	from category c, rating r 
	group by sector 
	order by avg(r.star) DESC;

-- 12. Output the person names of the executives that are affiliated with more than one company.
    create or replace view Q12(Name) as
	select distinct e.person
	from Executive e 
	where exists (
		select e1.person
		from Executive e1
		where e1.person = e.person and e1.code != e.code
	)
	order by e.person;
	

-- 13. Find all the companies with a registered address in Australia, in a Sector where there are no overseas companies in the same Sector. i.e., they are in a Sector that all companies there have local Australia address.
	create or replace view foreignsects as
	select distinct cat1.sector as sect
	from company c1, category cat1 
	where c1.code = cat1.code and c1.country != 'Australia';

	create or replace view localcodes as
	select cat.code as code, cat.sector as sector 
	from category cat 
	where not exists (select cat.sector 
		from foreignsects 
		where cat.sector = foreignsects.sect
	);

    create or replace view Q13(Code, Name, Address, Zip, Sector) as
	select c.code, c.name, c.address, c.zip, lc.sector 
	from company c, localcodes lc
	where c.code = lc.code;

-- 14. Calculate stock gains based on their prices of the first trading day and last trading day (i.e., the oldest "Date" and the most recent "Date" of the records stored in the ASX table). Order your result by Gain in descending order and then by Code in ascending order.
	create or replace view dates as 
	select a1.code, min(a1."Date") as begindate, max(a1."Date") as enddate
	from ASX a1
	group by a1.code;

    create or replace view Q14(Code, BeginPrice, EndPrice, Change, Gain) as 
	select a1.code, a1.price as beginprice, a2.price as endprice, a2.price-a1.price as change, 100*(a2.price-a1.price)/a1.price as gain 
	from ASX a1, ASX a2, dates d
	where a1.code = d.code and a2.code = d.code and a1."Date" = d.begindate and a2."Date" = d.enddate
	order by gain desc;

-- 15. For all the trading records in the ASX table, produce the following statistics as a database view (where Gain is measured in percentage). AvgDayGain is defined as the summation of all the daily gains (in percentage) then divided by the number of trading days (as noted above, the total number of days here should exclude the first trading day).
	create or replace view Q15(Code, MinPrice, AvgPrice, MaxPrice, MinDayGain, AvgDayGain, MaxDayGain) as 
	select code, min(price), avg(price), max(price), min(gain), avg(gain), max(gain) 
	from q7 
	group by code;

/*16. Create a trigger on the Executive table, to check and disallow any insert or update of a Person in the Executive table to be an executive of more than one company.*/ 
	create or replace function
		check_executives() returns trigger
	as $$
	begin
		if exists (select exec.person from executive exec where NEW.person = exec.person)
			then return null;
		end if;
	return NEW;
	end;
	$$ language 'plpgsql';

	create trigger check_exec
	before insert or update on executive
	for each row
	execute procedure check_executives();

/*17. Suppose more stock trading data are incoming into the ASX table. Create a trigger to increase the stock's rating (as Star's) to 5 when the stock has made a maximum daily price gain (when compared with the price on the previous trading day) in percentage within its sector. For example, for a given day and a given sector, if Stock A has the maximum price gain in the sector, its rating should then be updated to 5. If it happens to have more than one stock with the same maximum price gain, update all these stocks' ratings to 5. Otherwise, decrease the stock's rating to 1 when the stock has performed the worst in the sector in terms of daily percentage price gain. If there are more than one record of rating for a given stock that need to be updated, update (not insert) all these records. You may assume that there are at least two trading records for each stock in the existing ASX table, and do not worry about the case that when the ASX table is initially empty. 
	-- view which displays the highest gain per sector per day
	create or replace view maxgains as
	select gains."Date", cat.sector,  max(gains.gain) as max_gain 
	from q7 gains, category cat 
	where gains.code = cat.code 
	group by gains."Date", cat.sector 
	order by gains."Date";


	create or replace function 
		update_rating() returns trigger
	as $$
	begin
		if asxlog."Date" = maxgains."Date" and category.sector = maxgains.sector and q7.gains = maxgains.max_gain
			then update rating set star = 5 where code =*/ 
			

/*18. Stock price and trading volume data are usually incoming data and seldom involve updating existing data. However, updates are allowed in order to correct data errors. All such updates (instead of data insertion) are logged and stored in the ASXLog table. Create a trigger to log any updates on Price and/or Voume in the ASX table and log these updates (only for update, not inserts) into the ASXLog table. Here we assume that Date and Code cannot be corrected and will be the same as their original, old values. Timestamp is the date and time that the correction takes place. Note that it is also possible that a record is corrected more than once, i.e., same Date and Code but different Timestamp.*/
create or replace function 
	updatelog() returns trigger 
as $$
declare log_time timestamp := date_trunc('second', now());
declare log_date date := new."Date";
declare log_code char(3) := new.code;
declare old_volume integer;
declare old_price numeric;
begin
	select volume from asx where "Date" = log_date and code = log_code into old_volume;
	select price from asx where "Date" = log_date and code = log_code into old_price;
	insert into ASXLog values (log_time, log_date, log_code, old_volume, old_price);
	return new;
end;
$$ language plpgsql;

create trigger update_check before update 
on asx
for each row execute procedure updatelog();
