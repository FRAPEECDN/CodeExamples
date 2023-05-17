package dev.FRACDN;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;

public class Main {

    static void testIfStatement(int a, int b) {
        if (a > b) {
            System.out.println("a is bigger than b");
        } else {
            System.out.println("b is bigger than a");
        }
    }

    static void testLoopWhile(int a, int b) {
        int count = 0;
        while (a < b) {
            System.out.print("looping ");
            System.out.println(count);
            System.out.print("a is ");
            System.out.println(a);
            count++;
            a++;
        }
        System.out.println("while loop ended");
    }

    static void testLoopRepeat(int a, int b) {
        int count = 0;
        do {
            System.out.print("looping ");
            System.out.println(count);
            System.out.print("a is ");
            System.out.println(a);
            count++;
            a++;
        } while (a < b);
        System.out.println("repeat loop ended");
    }

    static void testForloop(int a, int b) {
        for (int idx = a; idx < b; idx++) {
            System.out.print("looping ");
            System.out.println(idx);

        }
        System.out.println("for loop ended");
    }
    public static void main(String[] args) {
        boolean b = false;

        byte by = 10;
        short s = (short) Byte.MAX_VALUE + 1;
        int i = (int) Short.MAX_VALUE + 1;
        long l = (long) Integer.MAX_VALUE + 1;

        float f = 343.12f;
        double d = 35438580.21473;

        char character = 'a';
        String aStr = "basic string value";

        // Basic data tests (supported in c/c++)
        System.out.println("Basic data types");
        System.out.println("~~~~~~~~~~~~~~~~");
        System.out.println("Integers");
        System.out.println("~~~~~~~~~~~~~~~~");
        System.out.println("Limit functions example (int type)");
        System.out.print("Minimum value for int: " + Integer.MIN_VALUE);
        System.out.println();
        System.out.print("Maximum value for int: " + Integer.MAX_VALUE);
        System.out.print("Int Hex function: " + Integer.toHexString(65));
        System.out.print("signed byte (1): " + by);
        System.out.print("signed short (2): " + s);
        System.out.print("signed int types (4): " + i);
        System.out.print("signed long (8): " + l);
        System.out.println("~~~~~~~~~~~~~~~~");
        System.out.println("Floating points");
        System.out.println("~~~~~~~~~~~~~~~~");
        System.out.print("float: " + f);
        System.out.print("double: " + d);
        System.out.print("Minimum value for double: " + Double.MIN_VALUE);
        System.out.print(" Exponent :" + Double.MIN_EXPONENT);
        System.out.print("Maximum value for double: " + Double.MAX_VALUE);
        System.out.print(" Exponent :" + Double.MAX_EXPONENT);
        System.out.print("Is double infinite support: " + Double.POSITIVE_INFINITY);
        System.out.println("~~~~~~~~~~~~~~~~");
        System.out.println("String and characters");
        System.out.print("Basic character: " + character);
        System.out.print("Basic string: " + aStr);
        System.out.println("~~~~~~~~~~~~~~~~");
        System.out.println("Boolean");
        System.out.print("boolean example: " + b);
        System.out.println("~~~~~~~~~~~~~~~~");

        // If testing 
        System.out.println("If statement test");
        System.out.println("~~~~~~~~~~~~~~~~");
        int noA = 78;
        int noB = 77;
        testIfStatement(noA, noB);
        noB = 79;
        testIfStatement(noA, noB);
        noA = 2;
        noB = 10;

        // Loop testing
        System.out.println("while loop test");
        System.out.println("~~~~~~~~~~~~~~~");
        testLoopWhile(noA, noB);
        System.out.println("do while test");
        System.out.println("~~~~~~~~~~~~~");
        testLoopRepeat(noA, noB);
        System.out.println("for test");
        System.out.println("~~~~~~~~");
        testForloop(noA, noB);

        // Inputs - Processing
        BufferedReader bfn = new BufferedReader(new InputStreamReader(System.in));

        int intInput = 0;
        do {
            try {
                System.out.println("Please provide a integer");
                intInput = Integer.parseInt(bfn.readLine());
            } catch (IOException | NumberFormatException e) {
                System.out.println("Error");
            }
        } while (intInput == 0);

        System.out.println("Provided the integer :" + intInput);
        Pattern pattern = Pattern.compile("^[a-z]+$");
        boolean result = false;
        String strInput = "";

        do {
            try {
                System.out.println("Please provide a small character string");
                strInput = bfn.readLine();
                result = pattern.matcher(strInput).find();
            } catch (IOException e) {
                System.out.println("Error");
            }
        } while (!result);
        System.out.println("Provided the string :" + strInput);

        // Collection example
        ArrayList<Integer> intCollection = new ArrayList<>(List.of(9, 8, 7, 6));
        System.out.println("Added to collection :" + intCollection.size() + " elements");
        System.out.println("Element in start is :" + intCollection.get(0));
        intCollection.remove(1);
        System.out.println("Added to collection :" + intCollection.size() + " elements");
        for (int idx : intCollection) {
            System.out.println(idx);
        }

        // Using record
        System.out.println("record usage");
        ExampleStruct exampleStruct = new ExampleStruct(5, "Hello");
        System.out.println(exampleStruct.a());
        System.out.println(exampleStruct.b());
        System.out.println(exampleStruct);
    }
}