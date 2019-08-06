-- table of movies with ratings and certain name
select m.title, m.year, m.content_rating, r.imdb_score from movie m, rating r where m.id = r.movie_id and m.title like '%Happy%';

-- get list of movies with their genres
select genrelist.title, genrelist.genre from genrelist where exists (select m.title, m.year, m.content_rating, r.imdb_score from movie m, rating r where genrelist.title = m.title and m.id = r.movie_id and m.title like '%Happy%');

-- get list of distinct movies fitting a certain pattern
select m.title from movie m where m.title like'%Happy%';

-- get list of movies from certain year range, order by rating
select m.title, m.year, m.content_rating, m.lang, r.imdb_score, r.num_voted_users from movie m, rating r where r.movie_id = m.id and m.year >= 2005 and m.year <= 2005 order by r.imdb_score DESC, r.num_voted_users DESC limit 10;

-- get list of movies who are adventure, scifi, and action genre
select m.title, m.year, m.content_rating, m.lang, r.imdb_score, r.num_voted_users from movie m, rating r where r.movie_id = m.id and m.year >= 2005 and m.year <= 2005 and exists (select m2.title, g2.genre from movie m2, genre g2 where m2.id = m.id and m2.id = g2.movie_id and g2.genre = 'Action') and exists (select m2.title, g2.genre from movie m2, genre g2 where m2.id = m.id and m2.id = g2.movie_id and g2.genre = 'Sci-Fi') and exists (select m2.title, g2.genre from movie m2, genre g2 where m2.id = m.id and m2.id = g2.movie_id and g2.genre = 'Adventure') order by r.imdb_score DESC, r.num_voted_users DESC limit 10;

select count(distinct(g.genre)) from (select m.title, m.year, m.content_rating, m.lang, r.imdb_score, r.num_voted_users from movie m, rating r, genre g where r.movie_id = m.id and m.year >= 2005 and m.year <= 2005 and g.movie_id = m.id and g.genre in ('Action', 'Adventure', 'Sci-Fi') order by r.imdb_score DESC, r.num_voted_users DESC limit 10) as meme;

-- get number of distinct titles
select count(distinct(meme.titles)) from (select m.title titles, m.year, m.content_rating, m.lang, r.imdb_score, r.num_voted_users from movie m, rating r, genre g where r.movie_id = m.id and m.year >= 2005 and m.year <= 2005 and g.movie_id = m.id and g.genre in ('Action', 'Adventure', 'Sci-Fi') order by r.imdb_score DESC, r.num_voted_users DESC limit 10) as meme

-- q3 meme
select gmovies.titles from (
	select meme.titles, count(distinct(meme.genre)) from (
		select g.genre genre, m.title titles, m.year, m.content_rating, m.lang, r.imdb_score, r.num_voted_users 
			from movie m, rating r, genre g 
			where r.movie_id = m.id and m.year >= 2005 and m.year <= 2005 and g.movie_id = m.id and g.genre in ('Action', 'Adventure', 'Sci-Fi') 
			order by r.imdb_score DESC, r.num_voted_users DESC limit 10
		)
		as meme group by meme.titles
	) 
	as gmovies where gmovies.count = 3;

select gmovies.titles, gmovies.year, gmovies.content_rating, gmovies.lang, gmovies.imdb_socre, gmovies.num_voted_users from (select meme.titles, meme.year, meme.content_rating, meme.lang, meme.imdb_score, meme.num_voted_users from (select g.genre, m.title titles, m.year, m.content_rating, m.lang, r.imdb_score, r.num_voted_users from movie m, rating r, genre g where r.movie_id = m.id and m.year >= 2005 and m.year <= 2005 and g.movie_id = m.id and g.genre in ('Action', 'Adventure', 'Sci-Fi') order by r.imdb_score DESC, r.num_voted_users DESC limit 10) as meme group by meme.titles) as gmovies where gmovies.count = 3;

select gmovies.titles, gmovies.year, gmovies.content_rating, gmovies.lang, gmovies.imdb_socre, gmovies.num_voted_users from (select meme.titles, meme.year, meme.content_rating, meme.lang, r.imdb_score, r.num_voted_users, count(distinct(meme.genre)) from (select g.genre, m.title titles, m.year, m.content_rating, m.lang, r.imdb_score, r.num_voted_users from movie m, rating r, genre g where r.movie_id = m.id and m.year >= 2005 and m.year <= 2005 and g.movie_id = m.id and g.genre in ('Action', 'Adventure', 'Sci-Fi') order by r.imdb_score DESC, r.num_voted_users DESC limit 10) as meme group by meme.titles) as gmovies where gmovies.count = 3;
