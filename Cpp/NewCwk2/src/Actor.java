/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author p0073862
 */
public class Actor {
    private String firstName;
    private String lastName;
    private int nbrMovies = 0;
    private Movie[] movies = new Movie[100];

    public Actor(String firstName, String lastName) {
        this.firstName = firstName;
        this.lastName = lastName;
    }

    public String getFullName() {
        return firstName + " " + lastName;
    }

    public String getFirstName() {
        return firstName;
    }

    public String getLastName() {
        return lastName;
    }

    public int getNbrMovies() {
        return nbrMovies;
    }

    public Movie getMovie(int i) {
        return movies[i];
    }

    public boolean equals(Actor actor) {
        return (actor.getFullName().equalsIgnoreCase(this.getFullName()));
    }

    public void addMovie(Movie movie) {
        movies[nbrMovies] = movie;
        nbrMovies++;
    }
}
