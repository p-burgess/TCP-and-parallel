/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author p0073862
 */
public class Movie {
    private String name;
    private Actor[] actors = new Actor[10];
    private int nbrActors = 0;

    public Movie(String name) {
        this.name = name;
    }

    
    public String getName() {
        return name;
    }

    public void addActor(Actor actor) {
        actors[nbrActors] = actor;
        nbrActors++;
    }

    public int getNbrActors() {
        return nbrActors;
    }

    public Actor getActor(int i) {
        return actors[i];
    }

    boolean containsActor(Actor actor) {
        for (int i=0; i<nbrActors;i++) {
            if (actors[i].equals(actor)) {
                return true;
            }
        }
        return false;
    }
}

