
import java.io.FileNotFoundException;
import java.util.Scanner;

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author p0073862
 */
public class Main {


    private static MovieBase movies = new MovieBase();
    private static Scanner scan = new Scanner(System.in);


    public static void main(String [] args) throws FileNotFoundException {
        String option;
        do {
            menu();
            option = getOption();
            if (option.equalsIgnoreCase("AM")) {
                System.out.print("Movie name ? ");
                String movieName = scan.nextLine();
                movies.addMovie(movieName);
            }
            else if (option.equalsIgnoreCase("AA")) {
                System.out.print("Actor's first name ? ");
                String firstName = scan.nextLine();
                System.out.print("Actor's last name ? ");
                String lastName = scan.nextLine();
                movies.addActor(firstName, lastName);
            }
            else if (option.equalsIgnoreCase("LM")) {
                int nbrMovies = movies.getNbrMovies();
                for (int i=0; i<nbrMovies; i++) {
                    String movieName = movies.getMovie(i);
                    System.out.println(movieName);
                }
            }
            else if (option.equalsIgnoreCase("LA")) {
                int nbrActors = movies.getNbrActors();
                for (int i=0; i<nbrActors; i++) {
                    String actor = movies.getActor(i);
                    System.out.println(actor);
                }
            }
            else if (option.equalsIgnoreCase("AC")) {
                System.out.print("Movie name ? ");
                String movie = scan.nextLine();
                System.out.print("Actor's first name ? ");
                String firstName = scan.nextLine();
                System.out.print("Actor's last name ? ");
                String lastName = scan.nextLine();
                if (!movies.addActorToMovie(movie, firstName, lastName)) {
                    System.out.println("Unable to add actor to cast.");
                    System.out.print("Check that both movie and actor have");
                    System.out.println(" been added to database");
                }
            }
            else if (option.equalsIgnoreCase("LAM")){
                System.out.print("Movie name ? ");
                String movie = scan.nextLine();
                int nbrActors = movies.getNbrActorsForMovie(movie);
                for (int i=0; i<nbrActors; i++) {
                    System.out.println(movies.getActorForMovie(movie, i));
                }
            }
            else if (option.equalsIgnoreCase("LMA")){
                System.out.print("Actor's first name ? ");
                String firstName = scan.nextLine();
                System.out.print("Actor's last name ? ");
                String lastName = scan.nextLine();
                int nbrMovies = movies.getNbrMoviesForActor(firstName,lastName);
                for (int i=0; i<nbrMovies; i++) {
                    String movie=movies.getMovieForActor(firstName,lastName,i);
                    System.out.println(movie);
                }
            }
            else if (option.equalsIgnoreCase("S")) {
                System.out.println("File name ? ");
                String fileName = scan.nextLine();
                movies.save(fileName);
            }
            else if (option.equalsIgnoreCase("L")) {
                System.out.println("File name ? ");
                String fileName = scan.nextLine();
                movies.load(fileName);
            }
        }
        while(!option.equalsIgnoreCase("Q"));
    }

    private static String getOption() {
        System.out.print("Your option ? ");
        return scan.nextLine();
    }

    private static void menu() {
        System.out.println("Options are ...");
        System.out.println("AM: add movie");
        System.out.println("AA: add actor");
        System.out.println("AC: add actor to cast of movie");
        System.out.println("LM: list movies");
        System.out.println("LA: list actors");
        System.out.println("LAM: list actors in movie");
        System.out.println("LMA: list movies with actor in cast");
        System.out.println("S: save to file");
        System.out.println("L: load from file");
        System.out.println("Q: quit program");
    }
}
