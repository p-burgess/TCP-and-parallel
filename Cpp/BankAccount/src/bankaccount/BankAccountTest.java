/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package bankaccount;

/**
 *
 * @author Arantza
 */

public class BankAccountTest {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        BankAccount myAccount = new BankAccount(1);
        BankAccount myOtherAccount = new BankAccount(2);

        myAccount.deposit(100);
        myOtherAccount.deposit(1000);
        System.out.println("The interest of my balance is " + myAccount.calculateInterest());
        System.out.println("The balance of account is " + myAccount.getBalance());

        myAccount.withdraw(25);
        System.out.println("The balance my acount is now " + myAccount.getBalance());

        myAccount.deposit(50);
        System.out.println("The Number of the Account is:  " + myAccount.getCustomerNumber());
        System.out.println("The balance my acount is now " + myAccount.getBalance());

        System.out.println("The interest on my other account is " + myOtherAccount.calculateInterest());
        myOtherAccount.deposit(myOtherAccount.calculateInterest());
          System.out.println("The Number of the other Account is:  " + myOtherAccount.getCustomerNumber());
          System.out.println("The balance of my other acount is now " + myOtherAccount.getBalance());

    }

}
