//N.B. You do NOT need to understand what the phrase
//"implements IMovieBase" means. Just don't delete it!

import java.util.Scanner;
import java.io.FileNotFoundException;
import java.io.FileInputStream;
import java.io.PrintWriter;
public class MovieBase implements IMovieBase {

    Movie[] movies = new Movie[100];
    int nbrMovies = 0;

    Actor[] actors = new Actor[100];
    int nbrActors = 0;


    /**
     * Add a movie to the database.
     * @param movie The name of the movie to add.
     */
    public void addMovie(String movie) {
        if (findMovie(movie) == null) {
         movies[nbrMovies] = new Movie(movie);
         nbrMovies++;
     }
    }

    /**
     * Get the number of movies in the database.
     * @return Number of movies in the database.
     */
    public int getNbrMovies() {
        return nbrMovies;
    }

    /**
     * Get the i'th movie in the database.
     * @param i Index of the movie (indices start at 0).
     * @return The name of the movie.
     */
    public String getMovie(int i) {
        return movies[i].getName();
    }

    /**
     * Add an actor to the database.
     * @param firstName First name of the actor.
     * @param lastName Last name of the actor.
     */
    public void addActor(String firstName, String lastName) {
        if (findActor(firstName, lastName) == null) {
            actors[nbrActors] = new Actor(firstName, lastName);
            nbrActors++;
        }
    }

    /**
     * Get the number of actors in the database.
     * @return Number of actors in the database.
     */
    public int getNbrActors() {
        return nbrActors;
    }

    /**
     * Get the full i'th actor in the database.
     * @param i Index of the actor (indices start at 0).
     * @return The full name of the actor.
     */
    public String getActor(int i) {
        return actors[i].getFullName();
    }

    /**
     * Get the first name of the i'th actor in the database.
     * @param i Index of the actor (indices start at 0).
     * @return The first name of the actor.
     */
    public String getActorFirstName(int i) {
        return actors[i].getFirstName();
    }

    /**
     * Get the last name of the i'th actor in the database.
     * @param i Index of the actor (indices start at 0).
     * @return The last name of the actor.
     */
    public String getActorLastName(int i) {
       return actors[i].getLastName();
    }


    /**
     * Add an actor to the cast of a movie.
     * The actor and movie must already be present in the database. The actor 
     * must not be already in the cast of the movie.
     * @param movieName The name of the movie.
     * @param firstName The first name of the actor.
     * @param lastName The last name of the actor.
     * @return true if the actor was successfully added to the cast of the 
     * movie (this happens if the movie and actor are already in the database,
     * and the actor has not already been added to the cast of the movie.
     */
    public boolean addActorToMovie(String movieName,String firstName,
                                                  String lastName){
       Actor actor = findActor(firstName, lastName);
       if(actor == null)
            return false;

       Movie movie = findMovie(movieName);
       if (movie == null)
           return false;

       if (isInCast(movieName, firstName, lastName))
           return false;

       movie.addActor(actor);
       actor.addMovie(movie);
       return true;

    }

    /**
     * Get the number of actors in the cast of a movie
     * @param movieName The name of the movie.
     * @return The number of actors in the movie's cast.
     */
    public int getNbrActorsForMovie(String movieName) {
        Movie movie = findMovie(movieName);
        if (movie == null)
            return 0;
        else
            return movie.getNbrActors();
    }

    /**
     * Get the i'th actor in the cast of a particular movie.
     * @param movieName The name of the movie.
     * @param i Index of the actor (indices start at 0). Note that this is the
     * index of the actor within this particular cast, not the index of the 
     * actor in the database.
     * @return The full name of the actor.
     */
    public String getActorForMovie(String movieName, int i) {
        Movie movie = findMovie(movieName);
        if (movie == null)
            return null;
        else {
            Actor actor= movie.getActor(i);
            return actor.getFullName();
        }
    }

    /**
     * Get the number of movies that an actor has appeared in.
     * 
     * @param firstName First name of the actor.
     * @param lastName Last name of the actor.
     * @return The number of movies that the actor has appeared in.
     */
    public int getNbrMoviesForActor(String firstName, String lastName) {
        Actor actor = findActor(firstName, lastName);
        if (actor == null)
            return 0;
        else
            return actor.getNbrMovies();
    }


    /**
     * Get the i'th movie that an actor has appeared in.
     * @param firstName First name of the actor.
     * @param lastName Last name of the actor.
     * @param i Index of the movie (indices start at 0). Note that this is the
     * index of the movie within the list associated with this particular actor,
     * not the index of the movie in the database.
     * @return The name of the movie
     */
    public String getMovieForActor(String firstName, String lastName, int i) {
        Actor actor = findActor(firstName, lastName);
        if (actor == null)
            return null;
        else {
            Movie movie = actor.getMovie(i);
            return movie.getName();
        }
    }

    /**
     * Save the database to a file.
     * 
     * @param fileName name of the file to save to.
     * @throws FileNotFoundException
     */
    public void save(String fileName) throws FileNotFoundException { 
        PrintWriter fileOut = new PrintWriter(fileName);
        fileOut.println();
        int i = 0;
        int j = 0;
        for (i = 0;i<nbrMovies;i++){
            fileOut.println("Movie");
            fileOut.println(movies[i].getName());
        }
        for (i = 0;i<nbrActors;i++){
            fileOut.println("Actor");
            fileOut.print(actors[i].getFirstName());
            fileOut.print(" ");
            fileOut.print(actors[i].getLastName());
            fileOut.println();
        }
        for (i = 0;i<nbrMovies;i++){
            if (movies[i].getNbrActors()>0){
                fileOut.println("CastOfMovie");
                fileOut.println(movies[i].getName());
                if (movies[i].getNbrActors()>0){
                    for (j = 0;j<movies[i].getNbrActors();j++){
                        fileOut.print(movies[i].getActor(j).getFirstName());
                        fileOut.print(" ");
                        fileOut.print(movies[i].getActor(j).getLastName());
                        fileOut.print(" ");
                    }
                    fileOut.println();
                }
            }
        }
        fileOut.close();
    }

    /**
     * Load the database from a file.
     * @param fileName Name of the file to load from.
     * @throws FileNotFoundException
     */
    public void load(String fileName) throws FileNotFoundException {
        FileInputStream fileIn = new FileInputStream(fileName);
        Scanner scan = new Scanner(fileIn);
        String field = scan.nextLine();
        String field1 = "";
        String field2 = "";
        String field3 = "";
        while (scan.hasNextLine()) {
            //number = 0;
           // field = scan.nextLine();
            if (field.equals("Movie")){
                field = scan.nextLine();
                this.addMovie(field);
                field=scan.nextLine();
            }
            // Array Arrays []= new Array [2];
            else if (field.equals("Actor")){
                field1 =scan.next();
                field2 =scan.next();
                this.addActor(field1, field2);
                field=scan.nextLine();
            }
            else if (field.equals("CastOfMovie")){
                field1 = scan.nextLine();
                field2 = scan.next();
                field3 = scan.next();
                this.addActorToMovie(field1, field2,field3);
                field=scan.nextLine();
            }
            else
                field=scan.nextLine();
        }
    }

    private Actor findActor(String firstName, String lastName) {
        for (int i=0;i<nbrActors;i++) {
            Actor actor = actors[i];
            if (actor.getFirstName().equals(firstName) &&
                   actor.getLastName().equals(lastName) ) {
                return actor;
            }
        }
        return null;
    }

    private Movie findMovie(String name) {
        for (int i=0;i<nbrMovies;i++) {
            Movie movie = movies[i];
            if (movie.getName().equalsIgnoreCase(name)) {
                return movie;
            }
        }
        return null;
    }

    public boolean isInCast(String movieName, String firstName,String lastName){
        Actor act = new Actor(firstName, lastName);
        Movie film = findMovie(movieName);
        if (film != null) {
            return (film.containsActor(act));
        }
        else
       return true;
       
    }
}
