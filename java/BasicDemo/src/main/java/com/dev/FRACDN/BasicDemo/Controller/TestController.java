package com.dev.FRACDN.BasicDemo.Controller;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * Root Controller that will report that server is available
 */
@RestController
@RequestMapping(path = "/test")
public class TestController {

    public static final String MESSAGE = "Spring Boot Server responded!";

    @GetMapping
    public String index() {
        return MESSAGE;
    }
}
