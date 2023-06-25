package com.dev.FRACDN.BasicDemo.Integration.Rest;

import com.dev.FRACDN.BasicDemo.Controller.CrudController;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.boot.test.web.client.TestRestTemplate;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpMethod;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;

import static org.assertj.core.api.Assertions.assertThat;

/**
 * Test the CrudController specifically
 */
@SpringBootTest(webEnvironment = SpringBootTest.WebEnvironment.RANDOM_PORT)
public class CrudRestTests {

    private static final String PATH = "/crud";
    private static final String BODY = "abc";
    private static final String ALL_RESULT = "[\"a for apple\",\" b for bananas\"]";

    @Autowired
    private TestRestTemplate template;

    @Test
    public void getAllTestString() {
        ResponseEntity<String> response = template.getForEntity(PATH, String.class);
        assertThat(response.getStatusCode()).isEqualTo(HttpStatus.OK);
        assertThat(response.getBody()).isEqualTo(ALL_RESULT);
        assertThat(response.getHeaders()).isNotEmpty();
    }
    @Test
    public void getSingleTestString() {
        ResponseEntity<String> response = template.getForEntity(PATH + "/{id}", String.class, 1);
        assertThat(response.getStatusCode()).isEqualTo(HttpStatus.OK);
        assertThat(response.getBody()).isEqualTo(CrudController.ONE_OBJECT);
        assertThat(response.getHeaders()).isNotEmpty();
    }

    @Test
    public void createTestString() {
        ResponseEntity<String> response = template.postForEntity(PATH, BODY, String.class);
        assertThat(response.getStatusCode()).isEqualTo(HttpStatus.CREATED);
        assertThat(response.getBody()).isEqualTo("1");
        assertThat(response.getHeaders()).isNotEmpty();
    }

    @Test
    public void updateTestString() {
        HttpEntity<String> requestEntity = new HttpEntity<>(BODY);
        ResponseEntity<String> response = template.exchange(PATH + "/{id}", HttpMethod.PUT, requestEntity, String.class, 1);
        assertThat(response.getStatusCode()).isEqualTo(HttpStatus.OK);
        assertThat(response.getBody()).isEqualTo(CrudController.ONE_OBJECT);
        assertThat(response.getHeaders()).isNotEmpty();
    }

    @Test
    public void deleteTestString() {
        ResponseEntity<Void> response = template.exchange(PATH + "/{id}", HttpMethod.DELETE,null, Void.class,1);
        assertThat(response.getStatusCode()).isEqualTo(HttpStatus.OK);
        assertThat(response.getBody()).isNull();
        assertThat(response.getHeaders()).isNotEmpty();
    }
}
