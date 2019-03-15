/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author p0073862
 */

public class GUIMain {
    public static void main(String[] args) {
        IMovieBase movieBase = new MovieBase();
        MBModel model = new MBModel(movieBase);
        MBView view = new MBView(model);
        MBController controller = new MBController(view, model);
        view.setController(controller);
    }
}

