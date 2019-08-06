/*
 Write an SQL view that produces a list of match report tuples, where each tuple contains the following fields:

    location ... the city where the match was played
    date ... the date on which the match was played
    team1 ... the first team in the match
    goals1 ... the goals scored by the first team
    team2 ... the second team in the match
    goals2 ... the goals scored by the secod team 

Normally, we would have the home team as the first team and the away team as the second team, but since this database doesn't enough have information to work out which is the home team, we simply put the teams in alphabetical order (i.e. team1 must come alphabetically before team2). */

drop view if exists match_teams;
create view match_teams
as
select m.id as id, m.city as city, t1.country as c1, t2.country as c2   
from Matches m, Teams t1, Teams t2, Involves i1, Involves i2 
where i1.match = m.id and i2.match = m.id and i1.team = t1.id and i2.team = t2.id and t1.country < t2.country
group by m.id;

drop view if exists team_goals;
create view team_goals
as 
select i.match as mid, t.country as country, count(g.id) as goals
from Players p, Teams t, Goals g, Involves i
where g.scoredBy = p.id and p.memberOf = t.id and i.team = t.id and g.scoredIn = i.match
group by i.match, t.country;

drop view if exists match_team_goals;
create view match_team_goals
as
select t1.mid as mid, t1.country as c1, t1.goals as g1, t2.country as c2, t2.goals as g2
from team_goals t1, team_goals t2
where t1.mid = t2.mid and t1.country < t2.country;

drop view if exists match_details;
create view match_details
as
select m.city, m.playedOn
from Matches m;



drop view if exists Q6;
create view Q6 
as

;
