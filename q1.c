#include <stdio.h>


double calculateRepayment(double loan, double interestRate, int years, double fixed_installment, double extra_payment) {
    // Base condition  that will stop the function if its true
    if (loan <= 0 || years <= 0) {
        return 0;  
    }

    
    double interest = loan * (interestRate / 100);
    double total_due = loan + interest;

    loan = total_due - fixed_installment - extra_payment;


    printf("Year %d: Remaining loan = %.2f\n", 4 - years, loan < 0 ? 0 : loan);

    
    return fixed_installment + extra_payment + calculateRepayment(loan, interestRate, years - 1, fixed_installment, extra_payment);
}

int main() {
    
    double initial_loan = 100000.0;
    double annual_interest_rate = 5.0;
    int total_years = 3;            
    double fixed_installment = 30000.0;     
    double extra_payment = 5000.0;         

    
    double total_repayment = calculateRepayment(initial_loan, annual_interest_rate, total_years, fixed_installment, extra_payment);
    printf("Total repayment over %d years: %.2f\n", total_years, total_repayment);

    return 0;
}

//  in this code   each call of the recursive function will represent different year of the loan paying procces
//using recursion will make it easier to understand as we will use less space and keep things simple
