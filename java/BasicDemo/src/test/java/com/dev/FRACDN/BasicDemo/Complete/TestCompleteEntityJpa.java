package com.dev.FRACDN.BasicDemo.Complete;

import com.dev.FRACDN.BasicDemo.Model.Entity.CompleteEntity;
import com.dev.FRACDN.BasicDemo.Repository.CompleteEntityRepository;
import com.dev.FRACDN.BasicDemo.Utilities.Base64Uuid;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.orm.jpa.DataJpaTest;
import org.springframework.boot.test.autoconfigure.orm.jpa.TestEntityManager;
import org.springframework.test.context.junit.jupiter.SpringExtension;

import java.util.UUID;

import static org.assertj.core.api.Assertions.assertThat;

/**
 * Unit test for repository (via EntityManager)
 */
@ExtendWith(SpringExtension.class)
@DataJpaTest
public class TestCompleteEntityJpa {

    @Autowired
    private CompleteEntityRepository repository;

    @Autowired
    private TestEntityManager entityManager;

    @BeforeEach
    public void setup() {
        entityManager.clear();
    }

    @Test
    public void testInsertNew() {
        String description = "12345";
        String publicIdGenerated = Base64Uuid.getInstance().newUuid();
        CompleteEntity newEntity = new CompleteEntity();
        newEntity.setPublicId(UUID.randomUUID().toString());
        newEntity.setDescription(description);
        CompleteEntity savedEntity = entityManager.persistFlushFind(newEntity);
        assertThat(savedEntity).isNotNull();
        assertThat(savedEntity.get_id()).isGreaterThan(0);
        assertThat(savedEntity.getPublicId()).isEqualTo(publicIdGenerated);
        assertThat(savedEntity.getDescription()).isEqualTo(description);
    }
}
