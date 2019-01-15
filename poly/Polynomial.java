package poly;

import java.io.IOException;
import java.util.Scanner;

/**
 * This class implements evaluate, add and multiply for polynomials.
 * 
 * @author runb-cs112
 *
 */
public class Polynomial {

	/**
	 * Reads a polynomial from an input stream (file or keyboard). The storage format
	 * of the polynomial is:
	 * <pre>
	 *     <coeff> <degree>
	 *     <coeff> <degree>
	 *     ...
	 *     <coeff> <degree>
	 * </pre>
	 * with the guarantee that degrees will be in descending order. For example:
	 * <pre>
	 *      4 5
	 *     -2 3
	 *      2 1
	 *      3 0
	 * </pre>
	 * which represents the polynomial:
	 * <pre>
	 *      4*x^5 - 2*x^3 + 2*x + 3 
	 * </pre>
	 * 
	 * @param sc Scanner from which a polynomial is to be read
	 * @throws IOException If there is any input error in reading the polynomial
	 * @return The polynomial linked list (front node) constructed from coefficients and
	 *         degrees read from scanner
	 */
	public static Node read(Scanner sc) 
			throws IOException {
		Node poly = null;
		while (sc.hasNextLine()) {
			Scanner scLine = new Scanner(sc.nextLine());
			poly = new Node(scLine.nextFloat(), scLine.nextInt(), poly);
			scLine.close();
		}
		return poly;
	}
	private static Node addToFront (float coeff, int degree, Node front) {
		front = new Node(coeff,degree,front);
		return front;
	}
	private static Node addToBack (float coeff, int degree, Node front) {
		if (front == null) {
			return addToFront(coeff, degree, front);
		} else {
			Node ptr = front;
			while (ptr.next != null) {
				ptr = ptr.next;
			}
			Node node = new Node(coeff, degree, null);
			ptr.next = node;
			return front;
		}
	}
	/**
	 * Returns the sum of two polynomials - DOES NOT change either of the input polynomials.
	 * The returned polynomial MUST have all new nodes. In other words, none of the nodes
	 * of the input polynomials can be in the result.
	 * 
	 * @param poly1 First input polynomial (front of polynomial linked list)
	 * @param poly2 Second input polynomial (front of polynomial linked list
	 * @return A new polynomial which is the sum of the input polynomials - the returned node
	 *         is the front of the result polynomial
	 */
	public static Node add(Node poly1, Node poly2) {
		Node sum = null;
		while(poly1 != null && poly2 != null) {
			if(poly1.term.degree == poly2.term.degree) {
				if(sum == null) {
					if(poly1.term.coeff + poly2.term.coeff != 0) {
						sum = new Node(poly1.term.coeff + poly2.term.coeff, poly1.term.degree, null);
					}
				}else {
					if(poly1.term.coeff + poly2.term.coeff != 0) {
						addToBack(poly1.term.coeff + poly2.term.coeff, poly1.term.degree, sum);
					}
				}
				poly1 = poly1.next;
				poly2 = poly2.next;
			}else if(poly1.term.degree < poly2.term.degree) {
				if(sum == null) {
					sum = new Node(poly1.term.coeff,poly1.term.degree,null);
				}else {
					addToBack(poly1.term.coeff, poly1.term.degree,sum);
				}
				poly1 = poly1.next;
			}else if(poly2.term.degree < poly1.term.degree) {
				if(sum == null) {
					sum = new Node(poly2.term.coeff,poly2.term.degree,null);
				}else {
					addToBack(poly2.term.coeff, poly2.term.degree,sum);
				}
				poly2 = poly2.next;
			}
		}
		while(poly1 != null && poly2 == null) {
			if(sum == null) {
				sum = new Node(poly1.term.coeff,poly1.term.degree,null);
			}else{
				addToBack(poly1.term.coeff, poly1.term.degree, sum);
			}
			poly1 = poly1.next;
		}
		while(poly2 != null && poly1 == null) {
			if(sum == null) {
				sum = new Node(poly2.term.coeff,poly2.term.degree,null);
			}else{
				addToBack(poly2.term.coeff, poly2.term.degree, sum);
			}
			System.out.println(Polynomial.toString(sum));
			poly2 = poly2.next;
		}
		return sum;
	}
	/**
	 * Returns the product of two polynomials - DOES NOT change either of the input polynomials.
	 * The returned polynomial MUST have all new nodes. In other words, none of the nodes
	 * of the input polynomials can be in the result.
	 * 
	 * @param poly1 First input polynomial (front of polynomial linked list)
	 * @param poly2 Second input polynomial (front of polynomial linked list)
	 * @return A new polynomial which is the product of the input polynomials - the returned node
	 *         is the front of the result polynomial
	 */
	public static Node multiply(Node poly1, Node poly2) {
		Node finalProduct = null;
		Node product = null;
		Node frontOf2 = poly2;
		while(poly1 != null) {	
			while(poly2 != null) {
				if(product == null) {
					product = new Node(poly1.term.coeff * poly2.term.coeff, poly1.term.degree + poly2.term.degree, null);
				}else {
					addToBack(poly1.term.coeff * poly2.term.coeff, poly1.term.degree + poly2.term.degree, product);
				}
				poly2 = poly2.next;
			}
			finalProduct = add(finalProduct, product);
			product = null;
			poly1 = poly1.next;
			poly2 = frontOf2;
		}
		return finalProduct;
	}

	/**
	 * Evaluates a polynomial at a given value.
	 * 
	 * @param poly Polynomial (front of linked list) to be evaluated
	 * @param x Value at which evaluation is to be done
	 * @return Value of polynomial p at x
	 */
	public static float evaluate(Node poly, float x) {
		float solution = 0;
		while(poly != null) {
			solution += (float) Math.pow(x, poly.term.degree) * poly.term.coeff;
			poly = poly.next;
		}
		return solution;
	}

	/**
	 * Returns string representation of a polynomial
	 * 
	 * @param poly Polynomial (front of linked list)
	 * @return String representation, in descending order of degrees
	 */
	public static String toString(Node poly) {
		if (poly == null) {
			return "0";
		} 

		String retval = poly.term.toString();
		for (Node current = poly.next ; current != null ;
				current = current.next) {
			retval = current.term.toString() + " + " + retval;
		}
		return retval;
	}	
}
