package com.dev.FRACDN.BasicDemo.Integration.Rest;

import com.dev.FRACDN.BasicDemo.Controller.TestController;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.boot.test.web.client.TestRestTemplate;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import static org.assertj.core.api.Assertions.assertThat;

/**
 * Test the TestController specifically
 */
@SpringBootTest(webEnvironment = SpringBootTest.WebEnvironment.RANDOM_PORT)
public class TestControllerRestTest {

    @Autowired
    private TestRestTemplate template;

    @Test
    public void getAllTestString() {
        ResponseEntity<String> response = template.getForEntity("/test", String.class);
        assertThat(response.getStatusCode()).isEqualTo(HttpStatus.OK);
        assertThat(response.getBody()).isEqualTo(TestController.MESSAGE);
        assertThat(response.getHeaders()).isNotEmpty();
    }
}
