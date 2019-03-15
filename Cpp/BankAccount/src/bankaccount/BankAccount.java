/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package bankaccount;
import java.util.Scanner;

/**
 *
 * @author Arantza
 *
 * edited by Paul Burgess
 * 10079217
 * 
 */
public class BankAccount {
    double balance;
    double interestRate; // interest Rate for the account
    int customerNumber;

    public static void main(String[] args)
    {

    }

    public BankAccount( int customerNumber) {
        balance = 0;
        interestRate = 0.06; // all the accounts have an 6% interest Rate
        this.customerNumber = customerNumber;


        }


   

 
    public double getBalance() {
        return balance;
    }

    public void deposit (double amount) {
        balance = balance + amount;
    }

    public void withdraw (double amount) {
        balance = balance - amount;

    }

    public double calculateInterest(){
        return  balance * interestRate;
    }

    public int getCustomerNumber(){
        return customerNumber;
    }
}

