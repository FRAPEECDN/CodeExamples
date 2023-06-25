package com.dev.FRACDN.BasicDemo.Service;


import com.dev.FRACDN.BasicDemo.Model.Entity.BasicEntity;
import com.dev.FRACDN.BasicDemo.Repository.BasicEntityRepository;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

/**
 * Basic demo service -> Basic business logic contained here
 */
@Service
@RequiredArgsConstructor
public class BasicService extends BaseService<BasicEntity> {

    public static final String FIND_BY_PUBLIC_ID_ERROR = "Basic %s not found";

    @Autowired
    private BasicEntityRepository basicEntityRepository;

    @Override
    protected String getFindByPublicIdError(String publicId) {
        return String.format(FIND_BY_PUBLIC_ID_ERROR, publicId);
    }

//    /**
//     * Return the DTO for the service request to get all basic objects in pageable format
//     * @param pageable pageable information
//     * @return response result of the service call
//     */
//    public BasicReturns getAll(final Pageable pageable) {
//        final Page<BasicEntity> basicEntityPage = this.basicEntityRepository.findAll(pageable);
//    }
//
//    /**
//     * Return a specific basic object if it exists
//     * @param publicId list of ids being requested
//     * @return response result of the service call
//     */
//    public BasicDTO getByPublicId(final String publicId) {
//        validateNotNullPublicId(publicId);
//        BasicEntity entity = findByPublicId(publicId, this.basicEntityRepository);
//    }
//
//    /**
//     * Return the dto for the service request to get a list of basic objects in pageable format
//     * @param publicIds list of ids being requested
//     * @param pageable pageable information
//     * @return response result of the service call
//     */
//    public BasicReturns getByPublicIds(final List<String> publicIds, final Pageable pageable) {
//        final Page<BasicEntity> basicEntityPage = this.basicEntityRepository.findByPublicIdIn(publicIds, pageable);
//    }
}
